#ifndef _H_MOTOR_CONTROLLER_
#define _H_MOTOR_CONTROLLER_

#include <array>

#include "dma.hpp"
#include "pin.hpp"
#include "pwm.hpp"
#include "events.hpp"
#include "motor-utils.hpp"
#include "motor-torque.hpp"
#include "motor-encoder.hpp"
#include "motor-driver.hpp"


namespace Motor {

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

  Controller(Instance instance, Torque &torque, Encoder &encoder,
             TIM_TypeDef *pwm_timer, uint32_t pwm_frequency,
             const Pin &pwm_pin1, const Pin &pwm_pin2);

  void init(void);


  //--------------------------------------------------
  //
  //  DRIVER
  //

  void stop(void) { _driver.stop(); }
  void brake(void)  { _driver.brake(); }
  void forward(int pwm_duty_pct) { _driver.forward(pwm_duty_pct); }
  void reverse(int pwm_duty_pct) { _driver.reverse(pwm_duty_pct); }


  //--------------------------------------------------
  //
  //  ENCODER
  //

  uint8_t encoder_valid_samples(void) const {
    return _encoder.valid_samples(_instance);
  }
  bool encoder_overcapture(void) const {
    return _encoder.overcapture(_instance);
  }
  void encoder_clear_overcapture(void) {
    _encoder.clear_overcapture(_instance);
  }
  float encoder_interval(Encoder::Averaging avg_mode) const {
    return _encoder.interval(avg_mode, _instance);
  }
  Encoder::Edge encoder_last_edge(void) const {
    return _encoder.last_edge(_instance);
  }
  float encoder_speed(Encoder::Averaging avg_mode) const {
    return _encoder.speed(avg_mode, _instance);
  }


  //--------------------------------------------------
  //
  //  TORQUE
  //

  float torque_adc_count(Torque::Averaging avg_mode) const {
    return _torque.adc_count(avg_mode, _instance);
  }
  float torque_current(Torque::Averaging avg_mode) const {
    return _torque.current(avg_mode, _instance);
  }
  float torque_torque(Torque::Averaging avg_mode) const {
    return _torque.torque(avg_mode, _instance);
  }

private:
  Instance _instance;
  Torque &_torque;
  Encoder &_encoder;
  Driver _driver;
};

};

#endif
