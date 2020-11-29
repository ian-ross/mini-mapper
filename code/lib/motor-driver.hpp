#ifndef _H_MOTOR_DRIVER_
#define _H_MOTOR_DRIVER_

#include "pin.hpp"
#include "pwm.hpp"


namespace Motor {

class Driver {
public:

  // TODO: ERROR HANDLING!

  // Two GPIOs, IN1 and IN2: must be able to PWM on both. This means
  // we need a timer with more than one PWM-capable channel.
  //
  // Both low => stop
  // Both high => brake
  //
  // One held high, negative PWM on the other => drive
  //    IN1 high => forward
  //    IN2 high => reverse
  //
  // So, need to be able to set timer so that pins are either constant
  // high (duty = 101%?) or low (duty = 0?).

  Driver(TIM_TypeDef *timer, uint32_t pwm_frequency,
         const Pin &in1, const Pin &in2);
  void init(void);

  void stop(void);
  void brake(void);
  void forward(int pwm_duty_pct);
  void reverse(int pwm_duty_pct);

#ifndef TEST
  // Kind of shonky, but it makes life oh so much easier when
  // testing...
private:
#endif

  PWM _pwm;
};

};

#endif
