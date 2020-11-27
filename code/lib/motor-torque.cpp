// Motor torque measurements
//
// The motor driver circuitry includes a current sense resistor and
// amplifier that allows motor coil current, and hence motor torque,
// to be measured.
//
// The current sense resistor has a value of 400 mΩ, the motor has a
// nominal maximum coil current of 250 mA, and the amplifier stage has
// a nominal gain of 30. This means that the voltage across the
// current sense resistor ranges from 0V (no coil current) to 0.1V
// (250 mA x 400 mΩ), and the voltage at the motor boards current
// sense output is ranges from 0V to 30 x 0.1V = 3V.
//
// (The correspondence between motor coil current and torque will have
// to be measured to determine calibration data.)
//
// The `Motor::Torque` class provides a facility to measure these
// torque and coil current values for up to two motors. This works by
// triggering ADC conversions for the analog inputs connected to the
// motor current sense inputs based on a timer, and collecting the
// conversion results using DMA. The raw ADC conversion results are
// collected in a circular buffer for smoothing and can be converted
// to current or torque values using calibration data.
//
// The DMA ISR for the DMA stream used should be set up to call the
// `dma_transfer_complete_irq` method of the `Motor::Torque` object.

#include <iostream>
#include <numeric>

#include "bsp-generic.h"
#include "errors.hpp"
#include "motor-torque.hpp"


static int pin_adc_channel(const ADC_TypeDef *adc, const Pin &pin);
static uint32_t adc_extsel_code(const TIM_TypeDef *_timer);
static void set_adc_sample_time(ADC_TypeDef *adc, uint8_t ch, uint8_t t);


// Simple constructor: most initialisation is done in the `init`
// method.

Motor::Torque::Torque(TIM_TypeDef *timer, ADC_TypeDef *adc, DMAChannel &dma,
                      int interval_ms, const Calibration &calib,
                      const Pin &left_pin, const Pin &right_pin)
    : _timer{timer}, _adc{adc}, _dmac{dma},
      _dma{(DMA_TypeDef *)(DMA1_BASE + (_dmac.dma - 1) * 0x0400UL)},
      _interval_ms{interval_ms}, _calib{calib},
      _pins{left_pin, right_pin} {}

// Initialise timer, ADC and DMA for motor coil sense current
// collection.

void Motor::Torque::init(void) {
  // Check timer selection (must be usable as a trigger source for the
  // ADC).
  if (_timer != TIM2 &&_timer != TIM5 &&
      _timer != TIM4 && _timer != TIM6) {
    fatal("timer is not supported as ADC trigger!");
    return;
  }

  // Check pin assignments to ADC inputs.
  int left_ch = pin_adc_channel(_adc, _pins[Instance::LEFT]);
  int right_ch = pin_adc_channel(_adc, _pins[Instance::RIGHT]);
  if (left_ch < 0 || right_ch < 0) {
    fatal("cannot use requested pins as ADC inputs!");
    return;
  }

  // Check ADC DMA stream/channel assignment.
  if (_dmac.dma != 2) {
    fatal("invalid DMA controller for ADC!");
    return;
  }
  bool valid = false;
  if (_adc == ADC1)
    valid = _dmac.channel == 0 &&
      (_dmac.stream == 0 || _dmac.stream == 4);
  if (_adc == ADC2)
    valid = _dmac.channel == 1 &&
      (_dmac.stream == 2 || _dmac.stream == 3);
  if (_adc == ADC3)
    valid = _dmac.channel == 2 &&
      (_dmac.stream == 0 || _dmac.stream == 1);
  if (!valid) {
    fatal("invalid DMA stream/channel for ADC!");
    return;
  }

  // Configure specified GPIO pins as analog inputs.
  _pins[Instance::LEFT].analog();
  _pins[Instance::RIGHT].analog();

  // Configure peripherals.
  configure_dma();
  configure_timer();
  configure_adc();
}


// DMA transfer complete IRQ.

void Motor::Torque::dma_transfer_complete_irq(void) {
  // DMA transfer error.
  if (READ_BIT(_dma->LISR ,DMA_LISR_TEIF0)) {
    WRITE_REG(_dma->LIFCR , DMA_LIFCR_CTEIF0);
    // TODO: DEAL WITH THIS.
    // _dma_error = true;
  }

  // DMA transfer complete.
  if (READ_BIT(_dma->LISR, DMA_LISR_TCIF0)) {
    WRITE_REG(_dma->LIFCR , DMA_LIFCR_CTCIF0);
    if (_sample_idx == -1) {
      // Initialise buffer contents on first sample.
      _samples[0].fill(_dma_buffer[0]);
      _samples[1].fill(_dma_buffer[1]);
      _sample_idx = 0;
    } else {
      _sample_idx = (_sample_idx + 1) % SAMPLE_COUNT;
      _samples[0][_sample_idx] = _dma_buffer[0];
      _samples[1][_sample_idx] = _dma_buffer[1];
    }
  }
}

uint16_t Motor::Torque::latest(Instance instance) {
  if (_sample_idx == -1)
    return 0;
  else
    return _samples[instance][_sample_idx];
}

float Motor::Torque::smoothed(Instance instance) {
  if (_sample_idx == -1)
    return 0.0f;
  else {
    const SampleBuffer &ss = _samples[instance];
    float accum = std::accumulate(ss.cbegin(), ss.cend(), 0.0f);
    return accum / SAMPLE_COUNT;
  }
}

void Motor::Torque::configure_dma(void) {
  // Enable DMA peripheral clock. (We've already checked that we're
  // using DMA2 in `init`.)
  __IO uint32_t tmpreg;
  SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_DMA2EN);
  tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_DMA2EN);
  (void)tmpreg;

  // Determine DMA stream peripheral address.
  DMA_Stream_TypeDef *stream =
    (DMA_Stream_TypeDef *)(DMA2_Stream0_BASE + _dmac.stream * 0x018UL);

  // Configure DMA transfer:
  //  - DMA transfer in circular mode to match with ADC configuration:
  //    DMA unlimited requests.
  //  - DMA transfer from ADC without address increment.
  //  - DMA transfer to memory with address increment.
  //  - DMA transfer from ADC by half-word to match with ADC configuration:
  //    ADC resolution 12 bits.
  //  - DMA transfer to memory by half-word to match with ADC conversion data
  //    buffer variable type: half-word.
  MODIFY_REG(stream->CR, DMA_SxCR_CHSEL, _dmac.channel << DMA_SxCR_CHSEL_Pos);
  MODIFY_REG(stream->CR,
             DMA_SxCR_DIR | DMA_SxCR_CIRC | DMA_SxCR_PINC | DMA_SxCR_MINC |
             DMA_SxCR_PSIZE | DMA_SxCR_MSIZE | DMA_SxCR_PL | DMA_SxCR_PFCTRL,
             0x00000000U |      // Direction: peripheral to memory
             DMA_SxCR_CIRC |    // Mode: circular
             0x00000000U |      // Peripheral: no increment
             DMA_SxCR_MINC |    // Memory: increment
             DMA_SxCR_PSIZE_0 | // Peripheral data align: halfword
             DMA_SxCR_MSIZE_0 | // Memory data align: halfword
             DMA_SxCR_PL_1);    // Priority: high

  // Set DMA transfer addresses.
  WRITE_REG(stream->PAR, (uintptr_t)&(_adc->DR));
  WRITE_REG(stream->M0AR, (uintptr_t)&_dma_buffer[0]);

  // Set DMA transfer size.
  MODIFY_REG(stream->NDTR, DMA_SxNDT, 2);

  // Enable DMA interrupts.
  NVIC_SetPriority(dma_irqn(_dmac), 1); // DMA IRQ lower priority than ADC IRQ.
  NVIC_EnableIRQ(dma_irqn(_dmac));
  SET_BIT(stream->CR, DMA_SxCR_TCIE);
  SET_BIT(stream->CR, DMA_SxCR_TEIE);

  // Enable DMA transfer.
  SET_BIT(stream->CR, DMA_SxCR_EN);
}


// Set up timer to fire trigger at required measurement interval.

void Motor::Torque::configure_timer(void) {
  // Retrieve timer clock source frequency. All the timers that we may
  // be using are on APB1. If APB1 prescaler != 1, timers have a x2
  // factor on their clock source.
  uint32_t apb1_prescaler = READ_BIT(RCC->CFGR, RCC_CFGR_PPRE1);
  uint32_t timer_clock_frequency =
    SystemCoreClock >> APBPrescTable[apb1_prescaler >> RCC_CFGR_PPRE1_Pos];
  if (apb1_prescaler != RCC_CFGR_PPRE1_DIV1) {
    timer_clock_frequency *= 2;
  }

  // Timer prescaler: we're measuring trigger intervals in
  // milliseconds, so setting the timer prescaler so that the timer
  // counter updates at 1 MHz will allow us to cover all intervals
  // that we need, even for 16-bit timers.
  const uint32_t prescaled_freq = 1000000;
  uint32_t timer_prescaler = timer_clock_frequency / prescaled_freq;

  // Timer reload value for required repeat period.
  uint32_t timer_reload = prescaled_freq / (1000 / _interval_ms);

  // Enable timer peripheral clock.
  uint32_t timer_periph_clock = 0;
  if (_timer == TIM2) timer_periph_clock = RCC_APB1ENR_TIM2EN;
  if (_timer == TIM4) timer_periph_clock = RCC_APB1ENR_TIM4EN;
  if (_timer == TIM5) timer_periph_clock = RCC_APB1ENR_TIM5EN;
  if (_timer == TIM6) timer_periph_clock = RCC_APB1ENR_TIM6EN;
  __IO uint32_t tmpreg;
  SET_BIT(RCC->APB1ENR, timer_periph_clock);
  tmpreg = READ_BIT(RCC->APB1ENR, timer_periph_clock);
  (void)tmpreg;

  // Configure timer timebase.
  WRITE_REG(_timer->PSC, timer_prescaler - 1);
  WRITE_REG(_timer->ARR, timer_reload - 1);
  MODIFY_REG(_timer->CR1, (TIM_CR1_DIR | TIM_CR1_CMS), 0);
  WRITE_REG(_timer->RCR, 0);

  // Configure timer TRGO signal on timer update event (i.e.
  // roll-over).
  MODIFY_REG(_timer->CR2, TIM_CR2_MMS, 0x02 << TIM_CR2_MMS_Pos);
}


// Set up ADC: configure input channels, sample timer, DMA connection,
// trigger source.

void Motor::Torque::configure_adc(void) {
  // Start appropriate ADC peripheral clock.
  uint32_t apb_bit;
  if (_adc == ADC1) apb_bit = RCC_APB2ENR_ADC1EN;
  if (_adc == ADC2) apb_bit = RCC_APB2ENR_ADC2EN;
  if (_adc == ADC3) apb_bit = RCC_APB2ENR_ADC3EN;
  __IO uint32_t tmpreg;
  SET_BIT(RCC->APB2ENR, apb_bit);
  tmpreg = READ_BIT(RCC->APB2ENR, apb_bit);
  (void)tmpreg;

  // Set ADC clock source: APB2 clock / 2.
  MODIFY_REG(ADC123_COMMON->CCR, ADC_CCR_ADCPRE, 0);

  // Set ADC regular continuous mode: off.
  MODIFY_REG(_adc->CR2, ADC_CR2_CONT, 0);

  // Set ADC regular trigger source: rising edge of timer TRGO.
  MODIFY_REG(_adc->CR2, ADC_CR2_EXTSEL,
             adc_extsel_code(_timer) << ADC_CR2_EXTSEL_Pos);
  MODIFY_REG(_adc->CR2, ADC_CR2_EXTEN, 0x01 << ADC_CR2_EXTEN_Pos);

  // Set ADC regular sequencer length: two channels (left and right).
  MODIFY_REG(_adc->SQR1, ADC_SQR1_L, (2 - 1) << ADC_SQR1_L_Pos);

  // Set ADC group regular sequence: left channel @ rank 0, right
  // channel @ rank 1.
  int left_ch = pin_adc_channel(_adc, _pins[Instance::LEFT]);
  int right_ch = pin_adc_channel(_adc, _pins[Instance::RIGHT]);
  MODIFY_REG(_adc->SQR3, ADC_SQR3_SQ1, left_ch << ADC_SQR3_SQ1_Pos);
  MODIFY_REG(_adc->SQR3, ADC_SQR3_SQ2, right_ch << ADC_SQR3_SQ2_Pos);

  // Set ADC input channel sample time: 56 cycles.
  // TODO: DECIDE ON SAMPLE TIME!
  set_adc_sample_time(_adc, left_ch, 0x03);
  set_adc_sample_time(_adc, right_ch, 0x03);

  // Enable multiple conversions.
  SET_BIT(_adc->CR1, ADC_CR1_SCAN);

  // Enable DMA transfer for ADC.
  SET_BIT(_adc->CR2, ADC_CR2_DMA);
  CLEAR_BIT(_adc->CR2, ADC_CR2_DDS);
}


void Motor::Torque::start(void) {
  // Enable timer and force update generation.
  SET_BIT(_timer->CR1, TIM_CR1_CEN);
  SET_BIT(_timer->EGR, TIM_EGR_UG);
}

void Motor::Torque::stop(void) {
  // Disable timer and force update generation.
  CLEAR_BIT(_timer->CR1, TIM_CR1_CEN);
  SET_BIT(_timer->EGR, TIM_EGR_UG);
}


//------------------------------------------------------------------------------
//
// TODO: FILL THIS IN
//

Motor::Torque::Calibration::Calibration() {

}

float Motor::Torque::Calibration::current(float adc_count) const {
  return 0.0;
}

float Motor::Torque::Calibration::torque(float adc_count) const {
  return 0.0;
}

//
//------------------------------------------------------------------------------


// ADC input pin assignments: pins ordered by ADC channel. Assignments
// for ADC1 and ADC2 are identical, while ADC3 is a little different.
// (Derived from Table 11 in STM32F767xx datasheet.)

static std::array<Pin, 16> adc12_pins = {
  PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7,  // CH 0-7
  PB0, PB1, PC0, PC1, PC2, PC3, PC4, PC5   // CH 8-15
};

static std::array<Pin, 16> adc3_pins = {
  PA0, PA1, PA2, PA3, PF6, PF7, PF8, PF9,  // CH 0-7
  PF10, PF3, PC0, PC1, PC2, PC3, PF4, PF5  // CH 8-15
};


// Work out ADC channel assignment for given GPIO pin, returning
// sentinel value if the pin isn't usable as an ADC input.

static int pin_adc_channel(const ADC_TypeDef *adc, const Pin &pin) {
  std::array<Pin, 16> &adc_pins = adc == ADC3 ? adc3_pins : adc12_pins;
  for (int i = 0; i < 16; ++i)
    if (pin == adc_pins[i])
      return i;
  return -1;
}


// Determine code for external triggering of ADC from timer TRGO
// signal (value to be inserted into EXTSEL field of ADC's CR2
// configuration register).

static uint32_t adc_extsel_code(const TIM_TypeDef *tim) {
  if (tim == TIM5) return 0x04;
  if (tim == TIM2) return 0x0B;
  if (tim == TIM4) return 0x0C;
  if (tim == TIM6) return 0x0D;
  return 0;
}


// Set ADC sample time for a given channel: these are split into two
// registers, channels 0-9 in SMPR2, and channels 10-18 in SMPR1.

static void set_adc_sample_time(ADC_TypeDef *adc, uint8_t ch, uint8_t t) {
  if (ch >= 10)
    MODIFY_REG(adc->SMPR1, 0x07 << 3 * (ch - 10), t << 3 * (ch - 10));
  else
    MODIFY_REG(adc->SMPR2, 0x07 << 3 * ch, t << 3 * ch);
}


//----------------------------------------------------------------------
//
//  TESTS
//

#ifdef TEST

#include <iostream>
#include "doctest.h"
#include "doctest/trompeloeil.hpp"
#include "events_mock.hpp"

extern char fatal_msg[];

TEST_CASE("Motor::Torque initialisation") {
  init_mock_mcu();
  fatal_msg[0] = '\0';

  Motor::Torque::Calibration cal;
  DMAChannel dma{2, 0, 0};

  SUBCASE("create torque monitor with invalid timer") {
    Motor::Torque torque(TIM1, ADC1, dma, 100 /* ms */, cal, PA4, PA5);
    torque.init();
    CHECK(strcmp(fatal_msg, "timer is not supported as ADC trigger!") == 0);
  }

  SUBCASE("create torque monitor with invalid pin assignments") {
    Motor::Torque torque(TIM6, ADC1, dma, 100 /* ms */, cal, PB3, PA5);
    torque.init();
    CHECK(strcmp(fatal_msg, "cannot use requested pins as ADC inputs!") == 0);
  }

  SUBCASE("create torque monitor with invalid DMA controller") {
    DMAChannel dma{1, 0, 0};
    Motor::Torque torque(TIM6, ADC1, dma, 100 /* ms */, cal, PA4, PA5);
    torque.init();
    CHECK(strcmp(fatal_msg, "invalid DMA controller for ADC!") == 0);
  }

  SUBCASE("create torque monitor with invalid DMA stream/channel") {
    DMAChannel dma{2, 2, 0};
    Motor::Torque torque(TIM6, ADC1, dma, 100 /* ms */, cal, PA4, PA5);
    torque.init();
    CHECK(strcmp(fatal_msg, "invalid DMA stream/channel for ADC!") == 0);
  }

  SUBCASE("create torque monitor with valid configuration") {
    Motor::Torque torque(TIM6, ADC1, dma, 100 /* ms */, cal, PA4, PA5);
    torque.init();
    CHECK(strcmp(fatal_msg, "") == 0);

    // Check GPIOs set to analog.
    CHECK(((GPIOA->MODER & (0x03 << (2 * 4))) >> (2 * 4)) == 0x03);
    CHECK(((GPIOA->MODER & (0x03 << (2 * 5))) >> (2 * 5)) == 0x03);

    // Check peripheral clocks: timer, ADC, DMA, GPIO ports.
    CHECK(READ_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM6EN) == RCC_APB1ENR_TIM6EN);
    CHECK(READ_BIT(RCC->APB2ENR, RCC_APB2ENR_ADC1EN) == RCC_APB2ENR_ADC1EN);
    CHECK(READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_DMA2EN) == RCC_AHB1ENR_DMA2EN);
    CHECK(READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN) == RCC_AHB1ENR_GPIOAEN);

    // Check timer timebase.
    uint32_t timer_clock_frequency = 54000000;
    uint32_t prescaler = timer_clock_frequency / 1000000 - 1;
    uint32_t reload = 1000000 / (1000 / 100) - 1;
    CHECK(TIM6->PSC == prescaler);
    CHECK(TIM6->ARR == reload);

    // Check timer mode.
    CHECK(READ_BIT(TIM6->CR1, TIM_CR1_CEN) == 0);
    CHECK(((TIM6->CR2 & TIM_CR2_MMS) >> TIM_CR2_MMS_Pos) == 0x02);

    // Check timer->ADC triggering.
    CHECK((READ_BIT(ADC1->CR2, ADC_CR2_EXTSEL) >> ADC_CR2_EXTSEL_Pos) == 0x0D);
    CHECK((READ_BIT(TIM6->CR2, TIM_CR2_MMS) >> TIM_CR2_MMS_Pos) == 0x02);

    // Check ADC channel sequencing and sample times.
    CHECK((READ_BIT(ADC1->SQR1, ADC_SQR1_L) >> ADC_SQR1_L_Pos) == (2 - 1));
    CHECK((READ_BIT(ADC1->SQR3, ADC_SQR3_SQ1) >> ADC_SQR3_SQ1_Pos) == 4);
    CHECK((READ_BIT(ADC1->SQR3, ADC_SQR3_SQ2) >> ADC_SQR3_SQ2_Pos) == 5);

    // Check DMA transfer count (proxy for other settings).
    CHECK(DMA2_Stream0->NDTR == 2);
  }
}

TEST_CASE("Motor::Torque start/stop") {
  init_mock_mcu();
  fatal_msg[0] = '\0';

  Motor::Torque::Calibration cal;
  DMAChannel dma{2, 0, 0};
  Motor::Torque torque(TIM6, ADC1, dma, 100 /* ms */, cal, PA4, PA5);
  torque.init();
  CHECK(strcmp(fatal_msg, "") == 0);

  SUBCASE("Start sampling") {
    torque.start();
    CHECK(strcmp(fatal_msg, "") == 0);
    CHECK((READ_BIT(TIM6->CR1, TIM_CR1_CEN) >> TIM_CR1_CEN_Pos) == 0x01);
  }

  SUBCASE("Start and stop sampling") {
    torque.start();
    torque.stop();
    CHECK(strcmp(fatal_msg, "") == 0);
    CHECK((READ_BIT(TIM6->CR1, TIM_CR1_CEN) >> TIM_CR1_CEN_Pos) == 0);
  }
}

TEST_CASE("Motor::Torque data collection") {
  init_mock_mcu();
  fatal_msg[0] = '\0';

  Motor::Torque::Calibration cal;
  DMAChannel dma{2, 0, 0};
  Motor::Torque torque(TIM6, ADC1, dma, 100 /* ms */, cal, PA4, PA5);
  torque.init();
  CHECK(strcmp(fatal_msg, "") == 0);

  SUBCASE("First sample") {
    // Before any samples.
    CHECK(torque.latest(Motor::LEFT) == 0);
    CHECK(torque.smoothed(Motor::LEFT) == 0);
    CHECK(torque.latest(Motor::RIGHT) == 0);
    CHECK(torque.smoothed(Motor::RIGHT) == 0);

    // Fill in a single sample.
    torque._dma_buffer[Motor::LEFT] = 123;
    torque._dma_buffer[Motor::RIGHT] = 456;

    // Set DMA transfer complete flag and call ISR.
    SET_BIT(DMA2->LISR, DMA_LISR_TCIF0);
    torque.dma_transfer_complete_irq();

    // Single sample should fill in all values in smoothing buffer.
    CHECK(torque.latest(Motor::LEFT) == 123);
    CHECK(torque.smoothed(Motor::LEFT) == 123);
    CHECK(torque.latest(Motor::RIGHT) == 456);
    CHECK(torque.smoothed(Motor::RIGHT) == 456);
  }

  SUBCASE("Multiple samples") {
    float totals[2] = { 0.0, 0.0 };
    float last[2];
    for (int sample = 0; sample < Motor::Torque::SAMPLE_COUNT; ++sample) {
      // Fill in a single sample.
      last[Motor::LEFT] = sample + 1;
      last[Motor::RIGHT] = 2 * sample + 1;
      torque._dma_buffer[Motor::LEFT] = sample + 1;
      torque._dma_buffer[Motor::RIGHT] = 2 * sample + 1;
      totals[Motor::LEFT] += last[Motor::LEFT];
      totals[Motor::RIGHT] += last[Motor::RIGHT];

      // Set DMA transfer complete flag and call ISR.
      SET_BIT(DMA2->LISR, DMA_LISR_TCIF0);
      torque.dma_transfer_complete_irq();
    }

    CHECK(torque.latest(Motor::LEFT) == last[Motor::LEFT]);
    CHECK(torque.latest(Motor::RIGHT) == last[Motor::RIGHT]);
    CHECK(torque.smoothed(Motor::LEFT) ==
          (totals[Motor::LEFT] / Motor::Torque::SAMPLE_COUNT));
    CHECK(torque.smoothed(Motor::RIGHT) ==
          (totals[Motor::RIGHT] / Motor::Torque::SAMPLE_COUNT));
  }
}

#endif
