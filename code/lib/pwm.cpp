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

void PWM::Init(void) {
  int timer_idx = 0;

  // In the specified pin usable as an output from the specified timer?
  bool pin_allowed = false;

  // Timer peripheral clock enable: register and bit position.
  volatile uint32_t *clock_reg;
  uint32_t clock_bit;

  // GPIO alternate function for using pin as timer output.
  GPIOAF af;

  if (timer == TIM10) {
    timer_idx = 10;
    pin_allowed = pin == PB8;
    clock_reg = &RCC->APB2ENR;
    clock_bit = RCC_APB2ENR_TIM10EN;
    af = GPIO_AF_3;
  } else if (timer == TIM11) {
    timer_idx = 11;
    pin_allowed = pin == PB9 || pin == PF7;
    clock_reg = &RCC->APB2ENR;
    clock_bit = RCC_APB2ENR_TIM11EN;
    af = GPIO_AF_3;
  } else if (timer == TIM13) {
    timer_idx = 13;
    pin_allowed = pin == PA6 || pin == PF8;
    clock_reg = &RCC->APB1ENR;
    clock_bit = RCC_APB1ENR_TIM13EN;
    af = GPIO_AF_9;
  } else if (timer == TIM14) {
    timer_idx = 14;
    pin_allowed = pin == PA7 || pin == PF9;
    clock_reg = &RCC->APB1ENR;
    clock_bit = RCC_APB1ENR_TIM14EN;
    af = GPIO_AF_9;
  }

  if (timer_idx == 0) {
    fatal("simple PWM not supported on timer");
    return;
  }
  if (!pin_allowed) {
    fatal("wrong pin for PWM timer");
    return;
  }

  // Enable timer peripheral clock.
  SET_BIT(*clock_reg, clock_bit);

  // Set up GPIO.
  pin.Output(GPIO_SPEED_VERY_HIGH, GPIO_TYPE_OPEN_DRAIN, GPIO_PUPD_PULL_UP);
  pin.Alternate(af);

  // Prescaler = 1 => timer runs at APBx clock frequency.
  timer->PSC = 0;

  // PWM mode 1 (start-aligned).
  MODIFY_REG(timer->CCMR1, TIM_CCMR1_OC1M_Msk, 0x06 << TIM_CCMR1_OC1M_Pos);
  SET_BIT(timer->CCER, TIM_CCER_CC1E);

  // Timer output to GPIO (via alternate function).
  MODIFY_REG(timer->OR, TIM11_OR_TI1_RMP_Msk, 0x00 << TIM11_OR_TI1_RMP_Pos);

  // Set preload controls.
  SET_BIT(timer->CCMR1, TIM_CCMR1_OC1PE);
  SET_BIT(timer->CR1, TIM_CR1_ARPE);

  // Auto-reload register: sets PWM period.
  timer->ARR = input_frequency() / frequency;

  // Capture/compare register: sets PWM duty cycle.
  timer->CCR1 = 0;
}


// Set PWM to inverted (active = low) or normal (active = high) mode.

void PWM::SetInverted(bool inv) {
  if (inv) {
    SET_BIT(timer->CCER, TIM_CCER_CC1P);
  } else {
    CLEAR_BIT(timer->CCER, TIM_CCER_CC1P);
  }
  SET_BIT(timer->EGR, TIM_EGR_UG);
}

// Timer input frequency: APB1 (TIM13 and TIM14) runs at 27 MHz and
// APB2 (TIM10 and TIM11) runs at 54 MHz.

uint32_t PWM::input_frequency(void) const {
  if (timer == TIM10 || timer == TIM11) {
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
    PWM pwm(TIM1, PA9, 1000);
    pwm.Init();
    CHECK(strcmp(fatal_msg, "") != 0);
  }

  SUBCASE("PWM initialisation: only appropriate pins allowed") {
    PWM pwm(TIM10, PA1, 10000);
    pwm.Init();
    CHECK(strcmp(fatal_msg, "") != 0);
  }

  SUBCASE("PWM initialisation: timer peripheral clock enabled") {
    PWM pwm(TIM10, PB8, 20000);
    pwm.Init();
    CHECK(READ_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM10EN) != 0);
  }

  SUBCASE("PWM initialisation: pin alternate function set correctly") {
    PWM pwm(TIM11, PF7, 30000);
    pwm.Init();
    CHECK(((GPIOF->AFR[0] & (0x0F << 28)) >> 28) == 0x03);
  }

  SUBCASE("PWM initialisation: timer frequency and duty cycle set correctly") {
    PWM pwm(TIM13, PA6, 30000);
    pwm.Init();
    // ARR = f_APB1 / 30 kHz = 27 MHz / 30 kHz = 900
    CHECK(TIM13->ARR == APB1_frequency / 30000);
    CHECK(TIM13->CCR1 == 0);
  }

  SUBCASE("PWM initialisation: PWM mode set correctly") {
    PWM pwm(TIM13, PA6, 30000);
    pwm.Init();
    CHECK(((TIM13->CCMR1 & TIM_CCMR1_OC1M_Msk) >> TIM_CCMR1_OC1M_Pos) == 6);
  }

  SUBCASE("PWM: set and retrieve duty cycle (count)") {
    PWM pwm(TIM13, PA6, 30000);
    pwm.Init();
    pwm.SetDuty(450);
    CHECK(TIM13->CCR1 == 450);
    CHECK(pwm.Duty() == 450);
  }

  SUBCASE("PWM: set and retrieve duty cycle (percentage)") {
    PWM pwm(TIM13, PA6, 30000);
    pwm.Init();
    pwm.SetDutyPct(25);
    CHECK(TIM13->CCR1 == TIM13->ARR / 4);
    CHECK(pwm.DutyPct() == 25);
  }

  SUBCASE("PWM: set inverse mode") {
    PWM pwm(TIM13, PA6, 30000);
    pwm.Init();
    CHECK(READ_BIT(TIM13->CCER, TIM_CCER_CC1P) == 0);
    pwm.SetInverted(true);
    CHECK(READ_BIT(TIM13->CCER, TIM_CCER_CC1P) != 0);
    pwm.SetInverted(false);
    CHECK(READ_BIT(TIM13->CCER, TIM_CCER_CC1P) == 0);
  }

  SUBCASE("PWM: start and stop timer") {
    PWM pwm(TIM13, PA6, 30000);
    pwm.Init();
    CHECK(READ_BIT(TIM13->CR1, TIM_CR1_CEN) == 0);
    pwm.On();
    CHECK(READ_BIT(TIM13->CR1, TIM_CR1_CEN) != 0);
    pwm.Off();
    CHECK(READ_BIT(TIM13->CR1, TIM_CR1_CEN) == 0);
  }
}

#endif
