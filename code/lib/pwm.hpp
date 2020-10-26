#ifndef _H_PWM_MULTI_
#define _H_PWM_MULTI_

#include <stdint.h>
#include <array>

#include "bsp-generic.h"
#include "pin.hpp"

class PWM {
public:
  enum Channel { CH1 = 0, CH2, CH3, CH4 };

  PWM(TIM_TypeDef *timer, uint32_t frequency, const Pin &pin1)
    : _timer{timer}, _frequency{frequency},
      _nchannels{1}, _pins{pin1, pin1, pin1, pin1} {}
  PWM(TIM_TypeDef *timer, uint32_t frequency,
           const Pin &pin1, const Pin &pin2)
    : _timer{timer}, _frequency{frequency},
      _nchannels{2}, _pins{pin1, pin2, pin2, pin2} { }
  PWM(TIM_TypeDef *timer, uint32_t frequency, const Pin &pin1,
           const Pin &pin2, const Pin &pin3)
    : _timer{timer}, _frequency{frequency},
      _nchannels{3}, _pins{pin1, pin2, pin3, pin3} {}
  PWM(TIM_TypeDef *timer, uint32_t frequency,
           const Pin &pin1, const Pin &pin2, const Pin &pin3, const Pin &pin4)
    : _timer{timer}, _frequency{frequency},
      _nchannels{4}, _pins{pin1, pin2, pin3, pin4} { }

  void init(void);

  bool is_on(void) const { return READ_BIT(_timer->CR1, TIM_CR1_CEN); }
  int reload_count(void) const { return _timer->ARR; }

  uint16_t duty(Channel ch) const { return _ccrs[ch]; }
  int duty_pct(Channel ch) const { return _ccrs[ch] * 100 / _timer->ARR; }

  void set_duty(Channel ch, uint16_t duty) {
    if (duty > _timer->ARR + 1) duty = 0;
    _ccrs[ch] = duty;
    SET_BIT(_timer->EGR, TIM_EGR_UG);
  }
  void set_duty_pct(Channel ch, int duty_pct) {
    if (duty_pct < 0) duty_pct = 0;
    if (duty_pct > 100) duty_pct = 100;
    _ccrs[ch] = duty_pct * _timer->ARR / 100;
    SET_BIT(_timer->EGR, TIM_EGR_UG);
  }

  void set(Channel ch) { set_duty(ch, is_inverted(ch) ? 0 : _timer->ARR + 1); }
  void reset(Channel ch) { set_duty(ch, is_inverted(ch) ? _timer->ARR + 1 : 0); }

  bool is_inverted(Channel ch) const;
  void set_inverted(Channel ch, bool inv);

  void on() {
    // Trigger update of timer parameters before enabling timer.
    SET_BIT(_timer->EGR, TIM_EGR_UG);
    SET_BIT(_timer->CR1, TIM_CR1_CEN);
  }
  void off() { CLEAR_BIT(_timer->CR1, TIM_CR1_CEN); }

  int channel_count(void) const { return _nchannels; }

  // Maximum number of channels for any supported timer peripheral.
  static const int MAX_NCHANNELS = 4;

private:

  TIM_TypeDef *_timer;                  // Timer peripheral.
  uint32_t _frequency;                  // PWM frequency.
  const int _nchannels;                 // Actual number of channels in use.
  std::array<Pin, MAX_NCHANNELS> _pins; // Pins assigned to PWM outputs.
  volatile uint32_t *_ccrs;

  uint32_t input_frequency(void) const;
};

#endif
