#include <iostream>
#include <cmath>

#include "bsp-generic.h"
#include "errors.hpp"
#include "motor-encoder.hpp"


static int pin_channel(const TIM_TypeDef *tim, const Pin &pin, GPIOAF *af);
static void enable_capture(TIM_TypeDef *tim, int ch);


Motor::Encoder::Encoder(TIM_TypeDef *timer,
                        const Pin &left_pin, const Pin &right_pin,
                        const WheelInfo &wheel_info) :
  Events::Consumer("Motor::Encoder"),
  _timer{timer}, _wheel_info{wheel_info}, _pins{left_pin, right_pin} { }


void Motor::Encoder::init(void) {
  // Check timer selection (must be 32-bit timer with multiple capture
  // channels).
  if (_timer != TIM2 &&_timer != TIM5) {
    fatal("timer is not supported for encoder capture!");
    return;
  }

  // Check pin assignments to timer channels.
  GPIOAF left_af, right_af;
  _chs[Instance::LEFT] = pin_channel(_timer, _pins[Instance::LEFT], &left_af);
  _chs[Instance::RIGHT] = pin_channel(_timer, _pins[Instance::RIGHT], &right_af);
  if (_chs[Instance::LEFT] < 0 || _chs[Instance::RIGHT] < 0) {
    fatal("cannot use requested pins as timer capture inputs!");
    return;
  }

  // Configure specified GPIO pins as appropriate alternate functions.
  _pins[Instance::LEFT].alternate(left_af);
  _pins[Instance::RIGHT].alternate(right_af);

  // Retrieve timer clock source frequency. All the timers that we may
  // be using are on APB1. If APB1 prescaler != 1, timers have a x2
  // factor on their clock source.
  uint32_t apb1_prescaler = READ_BIT(RCC->CFGR, RCC_CFGR_PPRE1);
  uint32_t timer_clock_frequency =
    SystemCoreClock >> APBPrescTable[apb1_prescaler >> RCC_CFGR_PPRE1_Pos];
  if (apb1_prescaler != RCC_CFGR_PPRE1_DIV1) {
    timer_clock_frequency *= 2;
  }

  // Timer prescaler: we set the timer prescaler so that the timer
  // counter updates at 1 MHz.
  const uint32_t prescaled_freq = 1000000;
  uint32_t timer_prescaler = timer_clock_frequency / prescaled_freq;

  // Enable timer peripheral clock.
  uint32_t timer_periph_clock = 0;
  if (_timer == TIM2) timer_periph_clock = RCC_APB1ENR_TIM2EN;
  if (_timer == TIM5) timer_periph_clock = RCC_APB1ENR_TIM5EN;
  __IO uint32_t tmpreg;
  SET_BIT(RCC->APB1ENR, timer_periph_clock);
  tmpreg = READ_BIT(RCC->APB1ENR, timer_periph_clock);
  (void)tmpreg;

  // Configure timer timebase, setting timer reload value to give
  // maximum roll-over period.
  WRITE_REG(_timer->PSC, timer_prescaler - 1);
  WRITE_REG(_timer->ARR, 0xFFFFFFFF);
  MODIFY_REG(_timer->CR1, (TIM_CR1_DIR | TIM_CR1_CMS), 0);
  WRITE_REG(_timer->RCR, 0);

  // Configure capture channels:
  enable_capture(_timer, _chs[Instance::LEFT]);
  enable_capture(_timer, _chs[Instance::RIGHT]);

  // Enable interrupts (DIER->CCxIE + NVIC stuff).
  NVIC_EnableIRQ(_timer == TIM2 ? TIM2_IRQn : TIM5_IRQn);
  SET_BIT(_timer->DIER, 0x01 << _chs[Instance::LEFT]);
  SET_BIT(_timer->DIER, 0x01 << _chs[Instance::RIGHT]);
}

static void enable_capture(TIM_TypeDef *tim, int ch) {
  // No input filtering (CCMRx->ICxF).
  // No transition prescaling (CCMRx->ICxPSC).
  // Select active input for capture channel (CCMRx->CCxS).
  // Positive and negative edge capture (CCER->CCxP and CCER->CCxNP).
  // Enable capture (CCER->CCxE).
  switch (ch) {
  case 1:
    MODIFY_REG(tim->CCMR1, TIM_CCMR1_IC1F, 0);
    MODIFY_REG(tim->CCMR1, TIM_CCMR1_IC1PSC, 0);
    MODIFY_REG(tim->CCMR1, TIM_CCMR1_CC1S, 0x01 << TIM_CCMR1_CC1S_Pos);
    SET_BIT(tim->CCER, TIM_CCER_CC1P | TIM_CCER_CC1NP | TIM_CCER_CC1E);
    break;

  case 2:
    MODIFY_REG(tim->CCMR1, TIM_CCMR1_IC2F, 0);
    MODIFY_REG(tim->CCMR1, TIM_CCMR1_IC2PSC, 0);
    MODIFY_REG(tim->CCMR1, TIM_CCMR1_CC2S, 0x01 << TIM_CCMR1_CC2S_Pos);
    SET_BIT(tim->CCER, TIM_CCER_CC2P | TIM_CCER_CC2NP | TIM_CCER_CC2E);
    break;

  case 3:
    MODIFY_REG(tim->CCMR2, TIM_CCMR2_IC3F, 0);
    MODIFY_REG(tim->CCMR2, TIM_CCMR2_IC3PSC, 0);
    MODIFY_REG(tim->CCMR2, TIM_CCMR2_CC3S, 0x01 << TIM_CCMR2_CC3S_Pos);
    SET_BIT(tim->CCER, TIM_CCER_CC3P | TIM_CCER_CC3NP | TIM_CCER_CC3E);
    break;

  case 4:
    MODIFY_REG(tim->CCMR2, TIM_CCMR2_IC4F, 0);
    MODIFY_REG(tim->CCMR2, TIM_CCMR2_IC4PSC, 0);
    MODIFY_REG(tim->CCMR2, TIM_CCMR2_CC4S, 0x01 << TIM_CCMR2_CC4S_Pos);
    SET_BIT(tim->CCER, TIM_CCER_CC4P | TIM_CCER_CC4NP | TIM_CCER_CC4E);
    break;
  }
}

void Motor::Encoder::timer_capture_irq(void) {
  for (auto i : Motor::INSTANCES) {
    // Channel capture? (SR->CCxIF: bits 1-4)
    if (READ_BIT(_timer->SR, 0x01 << _chs[i])) {
      // Read capture value (clears interrupt flag).
      uint32_t capture = (&_timer->CCR1)[_chs[i] - 1];

      // If overcapture (SR->CCxOF: bits 9-12), clear overcapture flag
      // and emit error event.
      if (READ_BIT(_timer->SR, 0x01 << (8 + _chs[i]))) {
        CLEAR_BIT(_timer->SR, 0x01 << (8 + _chs[i]));
        mgr->post(Events::ENCODER_OVERCAPTURE, i);
      } else {
        // Otherwise emit capture event with channel and timer value.
        mgr->post(Events::ENCODER_CAPTURE, i, capture);
      }
    }
  }
}


// Discard captured edges for an instance that lie further into the
// past than a specified discard horizon.

void Motor::Encoder::discard_edges(Instance i, uint32_t now) {
  uint8_t vs = _valid_samples[i];
  uint8_t idx = (_sample_idx[i] + SAMPLE_COUNT - vs) % SAMPLE_COUNT;
  for (uint8_t s = 0; s < vs; ++s) {
    if (now - _edge_times[i][idx] > EDGE_DISCARD_TIME) {
      _edge_times[i][idx] = 0;
      --_valid_samples[i];
    }
    idx = (idx + 1) % SAMPLE_COUNT;
  }
}


// Event dispatch: add captured edges.

void Motor::Encoder::dispatch(const Events::Event &e) {
  switch (e.tag) {
  case Events::ENCODER_OVERCAPTURE:
    // TODO: RECORD ERROR STATE SOMEWHERE...
    break;

  case Events::ENCODER_CAPTURE: {
    Instance i = static_cast<Instance>(e.param1);
    uint32_t capture = e.param2;
    discard_edges(i, capture);
    uint8_t is = _sample_idx[i];
    _edge_times[i][is] = capture;
    ++_valid_samples[i];
    if (_valid_samples[i] > SAMPLE_COUNT) _valid_samples[i] = SAMPLE_COUNT;
    _sample_idx[i] = (is + 1) % SAMPLE_COUNT;
    // TODO: SET last_edge
    break;
  }

  default:
    break;
  }
}

// STATES (SAMPLE_COUNT = 4, TIMEOUT = 50):
//
// Initial:
//   edge_times __ __ __ __   sample_idx=0  valid_samples=0
//
// Capture @ T=5
//   edge_times  5 __ __ __   sample_idx=1  valid_samples=1
//
// Capture @ T=8
//   edge_times  5  8 __ __   sample_idx=2  valid_samples=2
//
// Capture @ T=10
//   edge_times  5  8 10 __   sample_idx=3  valid_samples=3
//
// Capture @ T=12
//   edge_times  5  8 10 12   sample_idx=0  valid_samples=4
//
// Capture @ T=15
//   edge_times 15  8 10 12   sample_idx=1  valid_samples=4
//
// Calculate last interval: valid_samples >= 2 => OK
//   ip = ((sample_idx - 1) + 4) % 4 = 0
//   in = ((sample_idx - 2) + 4) % 4 = 3
//   interval = edge_times[ip] - edge_times[in] = 15 - 12 = 3
//
// Capture @ T=18
//   edge_times 15 18 10 12   sample_idx=2  valid_samples=4
//
// Capture @ T=66
//
// Check:
//  start = (sample_idx - valid_samples + 4) % 4 = (2 - 4 + 4) % 4 = 2
//  end = (sample_idx - 1 + 4) % 4 = (2 - 1 + 4) % 4 = 1
//  orig_valid_samples = valid_samples;
//  i = start;
//  for (s = 0; s < orig_valid_samples; ++s) {
//    if (now - edge_times[i] > TIMEOUT) {
//      edge_times[i] = X;
//      --valid_samples;
//    }
//    i = (i + 1) % 4;
//  }
//  => valid_samples = 1, edge_times = __ 18 __ __
//
//   edge_times __ 18 66 __   sample_idx=3  valid_samples=2
//
// Capture @ T=70
//   edge_times __ __ 66 70   sample_idx=0  valid_samples=2

const float EXP_WEIGHT_FACTOR = 0.9;

float Motor::Encoder::interval(Averaging avg_mode, Instance instance) const {
  switch (avg_mode) {
  case LAST_EDGES: {
    if (_valid_samples[instance] < 2) break;
    int ip = (_sample_idx[instance] + SAMPLE_COUNT - 1) % SAMPLE_COUNT;
    int in = (_sample_idx[instance] + SAMPLE_COUNT - 2) % SAMPLE_COUNT;
    return _edge_times[instance][ip] - _edge_times[instance][in];
  }

  case LAST_TOOTH: {
    if (_valid_samples[instance] < 3) break;
    int ip = (_sample_idx[instance] + SAMPLE_COUNT - 1) % SAMPLE_COUNT;
    int in = (_sample_idx[instance] + SAMPLE_COUNT - 3) % SAMPLE_COUNT;
    return _edge_times[instance][ip] - _edge_times[instance][in];
  }

  case BOXCAR: {
    if (_valid_samples[instance] < 2) break;
    int ip = (_sample_idx[instance] + SAMPLE_COUNT - 1) % SAMPLE_COUNT;
    float total = 0.0;
    for (int s = 0; s < _valid_samples[instance] - 1; ++s) {
      int in = (ip + SAMPLE_COUNT - 1) % SAMPLE_COUNT;
      total += _edge_times[instance][ip] - _edge_times[instance][in];
      ip = in;
    }
    return total / (_valid_samples[instance] - 1);
  }

  case EXPONENTIAL:  {
    if (_valid_samples[instance] < 2) break;
    int ip = (_sample_idx[instance] + SAMPLE_COUNT - 1) % SAMPLE_COUNT;
    float total = 0.0;
    float weight_total = 0.0;
    float weight = 1.0;
    for (int s = 0; s < _valid_samples[instance] - 1; ++s) {
      int in = (ip + SAMPLE_COUNT - 1) % SAMPLE_COUNT;
      total += weight * (_edge_times[instance][ip] - _edge_times[instance][in]);
      weight_total += weight;
      ip = in;
      weight *= EXP_WEIGHT_FACTOR;
    }
    return total / weight_total;
  }
  }

  return 0;
}

Motor::Encoder::Edge Motor::Encoder::last_edge(Instance instance) const {
  return _last_edge[instance];
}

float Motor::Encoder::speed(Averaging avg_mode, Instance instance) const {
  return 0;
}




// Work out timer channel assignment for given GPIO pin, returning
// sentinel value if the pin isn't usable as a timer capture input.
// Also fill in alternate function for timer channel on pin if the pin
// assignment is valid and `af` is non-null.

static int pin_channel(const TIM_TypeDef *tim, const Pin &pin, GPIOAF *af) {
  int retval = -1;

  // TIM2:
  //  - CH1: PA0 (AF1), PA5 (AF1), PA15 (AF1)
  //  - CH2: PA1 (AF1), PB3 (AF1)
  //  - CH3: PA2 (AF1), PB10 (AF1)
  //  - CH4: PA3 (AF1), PB11 (AF1)

  if (tim == TIM2) {
    if (pin == PA0 || pin == PA5 || pin == PA15) retval = 1;
    if (pin == PA1 || pin == PB3)                retval = 2;
    if (pin == PA2 || pin == PB10)               retval = 3;
    if (pin == PA3 || pin == PB11)               retval = 4;
    if (retval > 0 && af != nullptr) *af = GPIO_AF_1;
  }

  // TIM5:
  //  - CH1: PA0 (AF2), PH10 (AF2)
  //  - CH2: PA1 (AF2), PH11 (AF2)
  //  - CH3: PA2 (AF2), PH12 (AF2)
  //  - CH4: PA3 (AF2), PI0 (AF2)

  if (tim == TIM5) {
    if (pin == PA0 /* || pin == PH10 */) retval = 1;
    if (pin == PA1 /* || pin == PH11 */) retval = 2;
    if (pin == PA2 /* || pin == PH12 */) retval = 3;
    if (pin == PA3 /* || pin == PI0 */)  retval = 4;
    if (retval > 0 && af != nullptr) *af = GPIO_AF_2;
  }

  return retval;
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

TEST_CASE("Motor::Encoder initialisation") {
  init_mock_mcu();
  fatal_msg[0] = '\0';

  Motor::Encoder::WheelInfo wheel_info{
    .circumference = static_cast<uint32_t>(60.0 * M_PI * 1000.0),
    .encoder_gear_ratio = 48,
    .encoder_teeth = 12
  };

  SUBCASE("create motor encoder with invalid timer") {
    Motor::Encoder encoder(TIM6, PA4, PA5, wheel_info);
    encoder.init();
    CHECK(strcmp(fatal_msg, "timer is not supported for encoder capture!") == 0);
  }

  SUBCASE("create motor encoder with invalid pins") {
    Motor::Encoder encoder(TIM2, PA4, PA5, wheel_info);
    encoder.init();
    CHECK(strcmp(fatal_msg, "cannot use requested pins as timer capture inputs!") == 0);
  }

  SUBCASE("create motor encoder with valid configuration") {
    Motor::Encoder encoder(TIM2, PA2, PA3, wheel_info);
    encoder.init();
    CHECK(strcmp(fatal_msg, "") == 0);

    // Check GPIOs set to alternate function.
    CHECK(((GPIOA->MODER & (0x03 << (2 * 2))) >> (2 * 2)) == 0x02);
    CHECK((READ_BIT(GPIOA->AFR[0], GPIO_AFRL_AFRL2) >> GPIO_AFRL_AFRL2_Pos) == 0x01);
    CHECK(((GPIOA->MODER & (0x03 << (2 * 3))) >> (2 * 3)) == 0x02);
    CHECK((READ_BIT(GPIOA->AFR[0], GPIO_AFRL_AFRL3) >> GPIO_AFRL_AFRL3_Pos) == 0x01);

    // Check peripheral clocks: timer, GPIO ports.
    CHECK(READ_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM2EN) == RCC_APB1ENR_TIM2EN);
    CHECK(READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN) == RCC_AHB1ENR_GPIOAEN);

    // Check timer timebase.
    uint32_t timer_clock_frequency = 54000000;
    uint32_t prescaler = timer_clock_frequency / 1000000 - 1;
    CHECK(TIM2->PSC == prescaler);
    CHECK(TIM2->ARR == 0xFFFFFFFF);

    // Check timer mode.
    CHECK(READ_BIT(TIM2->CR1, TIM_CR1_CEN) == 0);
    CHECK(READ_BIT(TIM2->CR1, TIM_CR1_DIR) == 0);
    CHECK(READ_BIT(TIM2->CR1, TIM_CR1_CMS) == 0);

    // Check capture enable.
    CHECK((READ_BIT(TIM2->CCER, TIM_CCER_CC3E) >> TIM_CCER_CC3E_Pos) == 1);
    CHECK((READ_BIT(TIM2->CCER, TIM_CCER_CC4E) >> TIM_CCER_CC4E_Pos) == 1);

    // Check timer interrupt setup.
    CHECK((READ_BIT(TIM2->DIER, 0x01 << 3) >> 3) == 1);
    CHECK((READ_BIT(TIM2->DIER, 0x01 << 4) >> 4) == 1);
  }
}

TEST_CASE("Motor::Encode edge capture") {
  using trompeloeil::_;

  init_mock_mcu();
  fatal_msg[0] = '\0';

  MockEventWaiter waiter;
  Events::Manager ev(MockEventWaiter::wait_for_event);

  Motor::Encoder::WheelInfo wheel_info{
    .circumference = static_cast<uint32_t>(60.0 * M_PI * 1000.0),
    .encoder_gear_ratio = 48,
    .encoder_teeth = 12
  };
  Motor::Encoder encoder(TIM2, PA2, PA3, wheel_info);
  ev += encoder;
  encoder.init();

  MockEventConsumer consumer;
  ev += consumer;

  const int SAMPLE_COUNT = Motor::Encoder::SAMPLE_COUNT;

  SUBCASE("single capture") {
    ALLOW_CALL(consumer, dispatch(_));
    ev.drain();

    TIM2->CCR3 = 10;
    SET_BIT(TIM2->SR, TIM_SR_CC3IF);
    encoder.timer_capture_irq();
    REQUIRE_CALL(consumer, dispatch(_))
      .WITH(_1.tag == Events::ENCODER_CAPTURE &&
            _1.param1 == Motor::Instance::LEFT && _1.param2 == 10);
    ev.drain();
    CHECK(encoder._valid_samples[Motor::Instance::LEFT] == 1);
    CHECK(encoder._edge_times[Motor::Instance::LEFT][0] == 10);
  }

  SUBCASE("multiple captures") {
    ALLOW_CALL(consumer, dispatch(_));
    ev.drain();

    for (int t = 0; t < SAMPLE_COUNT + 1; ++t) {
      TIM2->CCR3 = t;
      SET_BIT(TIM2->SR, TIM_SR_CC3IF);
      encoder.timer_capture_irq();
      ev.drain();
    }

    CHECK(encoder._valid_samples[Motor::Instance::LEFT] == SAMPLE_COUNT);
    CHECK(encoder.interval(Motor::Encoder::LAST_EDGES, Motor::Instance::LEFT) == 1);
    CHECK(encoder.interval(Motor::Encoder::LAST_TOOTH, Motor::Instance::LEFT) == 2);
    CHECK(encoder.interval(Motor::Encoder::BOXCAR, Motor::Instance::LEFT) == 1);
  }

  SUBCASE("capture retirement") {
    ALLOW_CALL(consumer, dispatch(_));
    ev.drain();

    for (int t = 0; t < SAMPLE_COUNT + 1; ++t) {
      TIM2->CCR3 = t;
      SET_BIT(TIM2->SR, TIM_SR_CC3IF);
      encoder.timer_capture_irq();
      ev.drain();
    }
    TIM2->CCR3 = SAMPLE_COUNT + Motor::Encoder::EDGE_DISCARD_TIME + 100;
    SET_BIT(TIM2->SR, TIM_SR_CC3IF);
    encoder.timer_capture_irq();
    ev.drain();

    CHECK(encoder._valid_samples[Motor::Instance::LEFT] == 1);
  }
}

#endif
