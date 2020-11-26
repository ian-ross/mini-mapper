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

#include <numeric>

#include "bsp-generic.h"
#include "errors.hpp"
#include "motor-torque.hpp"


static int pin_adc_channel(const ADC_TypeDef *adc, const Pin &pin);


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

  // Configure ADC.

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
    int accum = std::accumulate(ss.cbegin(), ss.cend(), 0.0f);
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
    (DMA_Stream_TypeDef *)(DMA2_Stream0_BASE + (_dmac.stream - 1) * 0x018UL);

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
    SystemCoreClock >> APBPrescTable[apb1_prescaler >>  RCC_CFGR_PPRE1_Pos];
  if (apb1_prescaler != RCC_CFGR_PPRE1_DIV1) {
    timer_clock_frequency *= 2;
  }

  // Timer prescaler: we're measuring trigger intervals in
  // milliseconds, so setting the timer prescaler so that the timer
  // counter updates at 1 MHz will allow us to cover all intervals
  // that we need, even for 16-bit timers.
  uint32_t timer_prescaler = timer_clock_frequency / 1000000;

  // Timer reload value for required repeat period.
  uint32_t timer_reload =
    (_interval_ms * timer_clock_frequency) / (1000 * timer_prescaler);

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

  // Configure timer.
  WRITE_REG(_timer->PSC, timer_prescaler - 1);
  WRITE_REG(_timer->ARR, timer_reload - 1);
  MODIFY_REG(_timer->CR1, (TIM_CR1_DIR | TIM_CR1_CMS), 0);
  WRITE_REG(_timer->RCR, 0);
  // TODO: CHECK THIS...
  // XXX: STOPPED HERE!!!
  MODIFY_REG(_timer->CR2, TIM_CR2_MMS, TIM_CR2_MMS_1); // TRGO update

  // Enable timer interrupts for debugging.
  SET_BIT(_timer->DIER, TIM_DIER_UIE);
  // TODO: IRQ NUMBERS.
  NVIC_SetPriority(TIM2_IRQn, 0);
  NVIC_EnableIRQ(TIM2_IRQn);
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



//----------------------------------------------------------------------
//
//  TESTS
//

#ifdef TEST

#include <iostream>
#include "doctest.h"
#include "doctest/trompeloeil.hpp"
#include "events_mock.hpp"

// extern char fatal_msg[];


#endif
