#ifndef _H_MOTOR_
#define _H_MOTOR_

#include <array>

#include "dma.hpp"
#include "pin.hpp"
#include "pwm.hpp"
#include "events.hpp"


namespace Motor {

#if 0
class Controller {
public:

  // Just bundle up all the different components of this here? What
  // should the rest of the API for this look like? Should we just
  // pull everything from the classes above into here, so we just have
  // a single Motor class? From the point of view of the hardware
  // resources, it makes sense to split things up, but from the point
  // of view of an application or higher-level abstraction using this
  // (e.g. a constant speed PID motor controller), it makes more sense
  // to have everything together, because you'll want to be able to
  // use the motor speed as a control input for a control law that
  // runs the motor driver as its output.

  Controller(TIM_TypeDef *pwm_timer, int pwm_frequency, Pin &pwm_pin1, Pin &pwm_pin2,
             Pin &encoder_pin, TIM_TypeDef *encoder_timer,
             Pin &torque_pin, ADC_TypeDef *torque_adc, int torque_measure_interval);
};
#endif

};

#endif
