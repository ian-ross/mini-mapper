#include "motor-controller.hpp"


#if 0
// Just bundle up all the different components of this here? What
// should the rest of the API for this look like? Should we just pull
// everything from the classes above into here, so we just have a
// single Motor class? From the point of view of the hardware
// resources, it makes sense to split things up, but from the point of
// view of an application or higher-level abstraction using this (e.g.
// a constant speed PID motor controller), it makes more sense to have
// everything together, because you'll want to be able to use the
// motor speed as a control input for a control law that runs the
// motor driver as its output.

Motor::Controller::Controller
  (TIM_TypeDef *pwm_timer, int pwm_frequency, Pin &pwm_pin1, Pin &pwm_pin2,
   Pin &encoder_pin, TIM_TypeDef *encoder_timer,
   Pin &torque_pin, ADC_TypeDef *torque_adc, int torque_measure_interval) {}
#endif


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
