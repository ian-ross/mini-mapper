#ifndef _H_PWM_
#define _H_PWM_

#include <stdint.h>

#include "bsp-generic.h"
#include "pin.hpp"

class PWM {
public:
  PWM(TIM_TypeDef *timer, const Pin &pin, uint32_t frequency)
    : _timer{timer}, _pin{pin}, _frequency{frequency} { }

  void init(void);

  uint16_t duty(void) const { return _timer->CCR1; }
  int duty_pct(void) const { return _timer->CCR1 * 100 / _timer->ARR; }
  bool is_on(void) const { return READ_BIT(_timer->CR1, TIM_CR1_CEN); }
  bool is_inverted(void) const { return READ_BIT(_timer->CCER, TIM_CCER_CC1P); }
  int reload_count(void) const { return _timer->ARR; }
  void set_duty(uint16_t duty) {
    if (duty > _timer->ARR + 1) duty = 0;
    _timer->CCR1 = duty;
    SET_BIT(_timer->EGR, TIM_EGR_UG);
  }
  void set_duty_pct(int duty_pct) {
    if (duty_pct < 0) duty_pct = 0;
    if (duty_pct > 100) duty_pct = 100;
    _timer->CCR1 = duty_pct * _timer->ARR / 100;
    SET_BIT(_timer->EGR, TIM_EGR_UG);
  }
  void set_inverted(bool inv);

  void on() {
    // Trigger update of timer parameters before enabling timer.
    SET_BIT(_timer->EGR, TIM_EGR_UG);
    SET_BIT(_timer->CR1, TIM_CR1_CEN);
  }
  void off() { CLEAR_BIT(_timer->CR1, TIM_CR1_CEN); }


private:

  TIM_TypeDef *_timer;
  const Pin &_pin;
  uint32_t _frequency;

  uint32_t input_frequency(void) const;
};

#endif
