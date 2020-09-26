#ifndef _H_PWM_
#define _H_PWM_

#include <stdint.h>

#include "bsp-generic.h"
#include "pin.hpp"

class PWM {
public:
  PWM(TIM_TypeDef *t, const Pin &p, uint32_t f)
    : timer{t}, pin{p}, frequency{f} { }

  void Init(void);

  uint16_t Duty(void) const { return timer->CCR1; }
  int DutyPct(void) const { return timer->CCR1 * 100 / timer->ARR; }
  bool IsOn(void) const { return READ_BIT(timer->CR1, TIM_CR1_CEN); }
  bool IsInverted(void) const { return READ_BIT(timer->CCER, TIM_CCER_CC1P); }
  int ReloadCount(void) const { return timer->ARR; }
  void SetDuty(uint16_t duty) {
    if (duty > timer->ARR) duty = 0;
    timer->CCR1 = duty;
    SET_BIT(timer->EGR, TIM_EGR_UG);
  }
  void SetDutyPct(int duty_pct) {
    if (duty_pct < 0) duty_pct = 0;
    if (duty_pct > 100) duty_pct = 100;
    timer->CCR1 = duty_pct * timer->ARR / 100;
    SET_BIT(timer->EGR, TIM_EGR_UG);
  }
  void SetInverted(bool inv);

  void On() {
    // Trigger update of timer parameters before enabling timer.
    SET_BIT(timer->EGR, TIM_EGR_UG);
    SET_BIT(timer->CR1, TIM_CR1_CEN);
  }
  void Off() { CLEAR_BIT(timer->CR1, TIM_CR1_CEN); }


private:

  TIM_TypeDef *timer;
  const Pin &pin;
  uint32_t frequency;

  uint32_t input_frequency(void) const;
};

#endif
