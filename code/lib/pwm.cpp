#include "errors.hpp"
#include "pwm.hpp"

// From STM32F767ZI reference manual (Section 27.3.9):
//
// Pulse Width Modulation mode allows you to generate a signal with a
// frequency determined by the value of the TIMx_ARR register and a
// duty cycle determined by the value of the TIMx_CCRx register.
//
// The PWM mode can be selected independently on each channel (one PWM
// per OCx output) by writing ‘0110’ (PWM mode 1) or ‘0111’ (PWM mode
// 2) in the OCxM bits in the TIMx_CCMRx register. You must enable the
// corresponding preload register by setting the OCxPE bit in the
// TIMx_CCMRx register, and eventually the auto-reload preload
// register (in upcounting or center-aligned modes) by setting the
// ARPE bit in the TIMx_CR1 register.
//
// As the preload registers are transferred to the shadow registers
// only when an update event occurs, before starting the counter, you
// have to initialize all the registers by setting the UG bit in the
// TIMx_EGR register.
//
// The OCx polarity is software programmable using the CCxP bit in the
// TIMx_CCER register. It can be programmed as active high or active
// low. The OCx output is enabled by the CCxE bit in the TIMx_CCER
// register. Refer to the TIMx_CCERx register description for more
// details.
//
// In PWM mode (1 or 2), TIMx_CNT and TIMx_CCRx are always compared to
// determine whether TIMx_CNT ≤ TIMx_CCRx.
//
// The timer is able to generate PWM in edge-aligned mode only since
// the counter is upcounting.

void PWM::init(void) {
  // Check that simple PWM is supported on the specified timer.
  if (_timer != TIM2 && _timer != TIM3 && _timer != TIM4 && _timer != TIM5 &&
      _timer != TIM9 && _timer != TIM10 && _timer != TIM11 &&
      _timer != TIM12 && _timer != TIM13 && _timer != TIM14) {
    fatal("simple PWM not supported on timer");
    return;
  }

  // Check required channel count.
  int max_channels = 1;
  if (_timer == TIM2 || _timer == TIM3 || _timer == TIM4 || _timer == TIM5)
    max_channels = 4;
  if (_timer == TIM9 || _timer == TIM12)
    max_channels = 2;
  if (_nchannels > max_channels) {
    fatal("specified PWM channel count not supported on timer");
    return;
  }

  // Are the specified pins usable as outputs from the specified timer?
  bool pins_allowed = false;
  if (_timer == TIM2) {
    pins_allowed = _pins[0] == PA0 || _pins[0] == PA5 || _pins[0] == PA15;
    if (_nchannels >= 2)
      pins_allowed = pins_allowed && (_pins[1] == PA1 || _pins[1] == PB3);
    if (_nchannels >= 3)
      pins_allowed = pins_allowed && (_pins[2] == PA2 || _pins[2] == PB10);
    if (_nchannels == 4)
      pins_allowed = pins_allowed && (_pins[3] == PA3 || _pins[3] == PB11);
  }
  if (_timer == TIM3) {
    pins_allowed = _pins[0] == PA6 || _pins[0] == PB4 || _pins[0] == PC6;
    if (_nchannels >= 2)
      pins_allowed = pins_allowed &&
        (_pins[1] == PA7 || _pins[1] == PB5 || _pins[1] == PC7);
    if (_nchannels >= 3)
      pins_allowed = pins_allowed && (_pins[2] == PB0 || _pins[2] == PC8);
    if (_nchannels == 4)
      pins_allowed = pins_allowed && (_pins[3] == PB1 || _pins[3] == PC9);
  }
  if (_timer == TIM4) {
    pins_allowed = _pins[0] == PB6 || _pins[0] == PD12;
    if (_nchannels >= 2)
      pins_allowed = pins_allowed &&
        (_pins[1] == PA7 || _pins[1] == PB7 || _pins[1] == PD13);
    if (_nchannels >= 3)
      pins_allowed = pins_allowed && (_pins[2] == PB8 || _pins[2] == PD14);
    if (_nchannels == 4)
      pins_allowed = pins_allowed && (_pins[3] == PB9 || _pins[3] == PD15);
  }
  if (_timer == TIM5) {
    pins_allowed = _pins[0] == PA0 /* || _pins[0] == PH10 */ ;
    if (_nchannels >= 2)
      pins_allowed = pins_allowed && (_pins[1] == PA1 /* || _pins[1] == PH11 */);
    if (_nchannels >= 3)
      pins_allowed = pins_allowed && (_pins[2] == PA2 /* || _pins[2] == PH12 */);
    if (_nchannels == 4)
      pins_allowed = pins_allowed && (_pins[3] == PA3 /* || _pins[3] == PI0 */);
  }
  if (_timer == TIM9) {
    pins_allowed = _pins[0] == PA2 || _pins[0] == PE5;
    if (_nchannels >= 2)
      pins_allowed = pins_allowed && (_pins[1] == PA3 || _pins[1] == PE6);
  }
  if (_timer == TIM10) {
    pins_allowed = _pins[0] == PB8 || _pins[0] == PF6;
  }
  if (_timer == TIM11) {
    pins_allowed = _pins[0] == PB9 || _pins[0] == PF7;
  }
  if (_timer == TIM12) {
    pins_allowed = _pins[0] == PB14 /* || _pins[0] == PH6 */;
    if (_nchannels >= 2)
      pins_allowed = pins_allowed && (_pins[1] == PB15 /* || _pins[1] == PH9 */);
  }
  if (_timer == TIM13) {
    pins_allowed = _pins[0] == PA6 || _pins[0] == PF8;
  }
  if (_timer == TIM14) {
    pins_allowed = _pins[0] == PA7 || _pins[0] == PF9;
  }

  if (!pins_allowed) {
    fatal("wrong pins for PWM timer");
    return;
  }

  // Set up timer peripheral clock enable register and bit position
  // and alternate function settings.
  volatile uint32_t *clock_reg = &RCC->APB1ENR;
  uint32_t clock_bit;
  GPIOAF af;
  if (_timer == TIM9 || _timer == TIM10 || _timer == TIM11)
    clock_reg = &RCC->APB2ENR;
  if (_timer == TIM2)  { clock_bit = RCC_APB1ENR_TIM2EN;   af = GPIO_AF_1; }
  if (_timer == TIM3)  { clock_bit = RCC_APB1ENR_TIM3EN;   af = GPIO_AF_2; }
  if (_timer == TIM4)  { clock_bit = RCC_APB1ENR_TIM4EN;   af = GPIO_AF_2; }
  if (_timer == TIM5)  { clock_bit = RCC_APB1ENR_TIM5EN;   af = GPIO_AF_2; }
  if (_timer == TIM9)  { clock_bit = RCC_APB2ENR_TIM9EN;   af = GPIO_AF_3; }
  if (_timer == TIM10) { clock_bit = RCC_APB2ENR_TIM10EN;  af = GPIO_AF_3; }
  if (_timer == TIM11) { clock_bit = RCC_APB2ENR_TIM11EN;  af = GPIO_AF_3; }
  if (_timer == TIM12) { clock_bit = RCC_APB1ENR_TIM12EN;  af = GPIO_AF_9; }
  if (_timer == TIM13) { clock_bit = RCC_APB1ENR_TIM13EN;  af = GPIO_AF_9; }
  if (_timer == TIM14) { clock_bit = RCC_APB1ENR_TIM14EN;  af = GPIO_AF_9; }

  // Enable timer peripheral clock.
  SET_BIT(*clock_reg, clock_bit);

  // Set up GPIOs.
  for (int i = 0; i < _nchannels; ++i) {
    _pins[i].output(GPIO_SPEED_VERY_HIGH, GPIO_TYPE_OPEN_DRAIN,
                    GPIO_PUPD_PULL_UP);
    _pins[i].alternate(af);
  }

  // Point to base CCR to be able to index by channel.
  _ccrs = &_timer->CCR1;

  // Prescaler = 1 => timer runs at APBx clock frequency.
  _timer->PSC = 0;

  // PWM mode 1 (start-aligned).
  MODIFY_REG(_timer->CCMR1, TIM_CCMR1_OC1M_Msk, 0x06 << TIM_CCMR1_OC1M_Pos);
  SET_BIT(_timer->CCER, TIM_CCER_CC1E);
  if (_nchannels >= 2) {
    MODIFY_REG(_timer->CCMR1, TIM_CCMR1_OC2M_Msk, 0x06 << TIM_CCMR1_OC2M_Pos);
    SET_BIT(_timer->CCER, TIM_CCER_CC2E);
  }
  if (_nchannels >= 3) {
    MODIFY_REG(_timer->CCMR2, TIM_CCMR2_OC3M_Msk, 0x06 << TIM_CCMR2_OC3M_Pos);
    SET_BIT(_timer->CCER, TIM_CCER_CC3E);
  }
  if (_nchannels == 4) {
    MODIFY_REG(_timer->CCMR2, TIM_CCMR2_OC4M_Msk, 0x06 << TIM_CCMR2_OC4M_Pos);
    SET_BIT(_timer->CCER, TIM_CCER_CC4E);
  }

  // Timer option registers.
  if (_timer == TIM2) _timer->OR = 0x0000;
  if (_timer == TIM5) _timer->OR = 0x0000;
  if (_timer == TIM11)
    MODIFY_REG(_timer->OR, TIM11_OR_TI1_RMP_Msk, 0x00 << TIM11_OR_TI1_RMP_Pos);

  // Set preload controls.
  SET_BIT(_timer->CCMR1, TIM_CCMR1_OC1PE);
  if (_nchannels >= 2) SET_BIT(_timer->CCMR1, TIM_CCMR1_OC2PE);
  if (_nchannels >= 3) SET_BIT(_timer->CCMR2, TIM_CCMR2_OC3PE);
  if (_nchannels == 4) SET_BIT(_timer->CCMR2, TIM_CCMR2_OC4PE);
  SET_BIT(_timer->CR1, TIM_CR1_ARPE);

  // Auto-reload register: sets PWM period.
  _timer->ARR = input_frequency() / _frequency;

  // Capture/compare registers: set PWM duty cycles.
  _ccrs[CH1] = 0;
  _ccrs[CH2] = 0;
  _ccrs[CH3] = 0;
  _ccrs[CH4] = 0;
}


// Check whether PWM is inverted (active = low) or normal (active =
// high).

bool PWM::is_inverted(Channel ch) const {
  int flag = 4 * (int)ch + TIM_CCER_CC1P;
  return READ_BIT(_timer->CCER, flag);
}

// Set PWM to inverted (active = low) or normal (active = high) mode.

void PWM::set_inverted(Channel ch, bool inv) {
  int flag = 4 * (int)ch + TIM_CCER_CC1P;
  if (inv) {
    SET_BIT(_timer->CCER, flag);
  } else {
    CLEAR_BIT(_timer->CCER, flag);
  }
  SET_BIT(_timer->EGR, TIM_EGR_UG);
}

// Timer input frequency: APB1 (TIM13 and TIM14) runs at 27 MHz and
// APB2 (TIM10 and TIM11) runs at 54 MHz.

uint32_t PWM::input_frequency(void) const {
  if (_timer == TIM10 || _timer == TIM11) {
    return APB2_frequency;
  } else {
    return APB1_frequency;
  }
}


//----------------------------------------------------------------------
//
//  TESTS
//

#ifdef TEST

#include <cstring>
#include <iostream>
#include "doctest.h"

extern char fatal_msg[];

TEST_CASE("PWM") {
  init_mock_mcu();
  fatal_msg[0] = '\0';

  SUBCASE("PWM initialisation: only appropriate timers allowed") {
#define TIMER_INIT_CHECK(var, tim)                                             \
  PWM var(TIM##tim, 1000, PA9);                                                     \
  var.init();                                                                  \
  CHECK(strcmp(fatal_msg, "simple PWM not supported on timer") == 0);          \
  fatal_msg[0] = '\0';

    TIMER_INIT_CHECK(pwm1, 1);
    TIMER_INIT_CHECK(pwm2, 6);
    TIMER_INIT_CHECK(pwm3, 7);
    TIMER_INIT_CHECK(pwm4, 8);
  }

  SUBCASE("PWM initialisation: maximum number of channels respected") {
#define CHANNEL_INIT_CHECK(var, tim, ...)                                      \
  PWM var(TIM##tim, 1000, __VA_ARGS__);                                             \
  var.init();                                                                  \
  CHECK(strcmp(fatal_msg,                                                      \
               "specified PWM channel count not supported on timer") == 0);    \
  fatal_msg[0] = '\0';

    CHANNEL_INIT_CHECK(pwm1, 10, PA1, PB2);
    CHANNEL_INIT_CHECK(pwm2, 9, PA1, PB2, PC3);
  }

  SUBCASE("PWM initialisation: reduced channel count allowed") {
    PWM pwm3(TIM2, 10000, PA0, PA1);
    pwm3.init();
    CHECK(strcmp(fatal_msg, "") == 0);
  }

  SUBCASE("PWM initialisation: only appropriate pins allowed") {
#define PIN_INIT_CHECK(var, tim, ...)                                          \
  PWM var(TIM##tim, 1000, __VA_ARGS__);                                             \
  var.init();                                                                  \
  CHECK(strcmp(fatal_msg, "wrong pins for PWM timer") == 0);                   \
  fatal_msg[0] = '\0';

    PIN_INIT_CHECK(pwm1, 10, PA1);
    PIN_INIT_CHECK(pwm2, 5, PA0, PA1, PA3);
    PIN_INIT_CHECK(pwm3, 9, PE5, PA4);
    PIN_INIT_CHECK(pwm4, 2, PA5, PA1, PB10, PB12);
  }

  SUBCASE("PWM initialisation: timer peripheral clock enabled") {
#define CLOCK_INIT_CHECK(var, tim, clock, ...)                                 \
  PWM var(TIM##tim, 1000, __VA_ARGS__);                                        \
  var.init();                                                                  \
  CHECK(READ_BIT(RCC->APB##clock##ENR, RCC_APB##clock##ENR_TIM##tim##EN) != 0);

    CLOCK_INIT_CHECK(pwm1, 10, 2, PB8);
    CLOCK_INIT_CHECK(pwm2, 2, 1, PA0);
    CLOCK_INIT_CHECK(pwm3, 4, 1, PB6, PB7, PB8, PB9);
    CLOCK_INIT_CHECK(pwm4, 9, 2, PA2);
  }

  SUBCASE("PWM initialisation: pin alternate function set correctly") {
    PWM pwm(TIM11, 30000, PF7);
    pwm.init();
    CHECK(((GPIOF->AFR[0] & (0x0F << 28)) >> 28) == 0x03);
  }

  SUBCASE("PWM initialisation: timer frequency and duty cycle set correctly") {
    PWM pwm(TIM13, 30000, PA6);
    pwm.init();
    // ARR = f_APB1 / 30 kHz = 27 MHz / 30 kHz = 900
    CHECK(TIM13->ARR == APB1_frequency / 30000);
    CHECK(TIM13->CCR1 == 0);
  }

  SUBCASE("PWM initialisation: PWM mode set correctly") {
    PWM pwm(TIM13, 30000, PA6);
    pwm.init();
    CHECK(((TIM13->CCMR1 & TIM_CCMR1_OC1M_Msk) >> TIM_CCMR1_OC1M_Pos) == 6);
  }

  SUBCASE("PWM: set and retrieve duty cycle (count)") {
    PWM pwm(TIM13, 30000, PA6);
    pwm.init();
    pwm.set_duty(PWM::CH1, 450);
    CHECK(TIM13->CCR1 == 450);
    CHECK(pwm.duty(PWM::CH1) == 450);
  }

  SUBCASE("PWM: set and retrieve duty cycle (percentage)") {
    PWM pwm(TIM13, 30000, PA6);
    pwm.init();
    pwm.set_duty_pct(PWM::CH1, 25);
    CHECK(TIM13->CCR1 == TIM13->ARR / 4);
    CHECK(pwm.duty_pct(PWM::CH1) == 25);
  }

  SUBCASE("PWM: set inverse mode") {
    PWM pwm(TIM13, 30000, PA6);
    pwm.init();
    CHECK(READ_BIT(TIM13->CCER, TIM_CCER_CC1P) == 0);
    pwm.set_inverted(PWM::CH1, true);
    CHECK(READ_BIT(TIM13->CCER, TIM_CCER_CC1P) != 0);
    pwm.set_inverted(PWM::CH1, false);
    CHECK(READ_BIT(TIM13->CCER, TIM_CCER_CC1P) == 0);
  }

  SUBCASE("PWM: start and stop timer") {
    PWM pwm(TIM13, 30000, PA6);
    pwm.init();
    CHECK(READ_BIT(TIM13->CR1, TIM_CR1_CEN) == 0);
    pwm.on();
    CHECK(READ_BIT(TIM13->CR1, TIM_CR1_CEN) != 0);
    pwm.off();
    CHECK(READ_BIT(TIM13->CR1, TIM_CR1_CEN) == 0);
  }
}

#endif
