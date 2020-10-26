#ifndef _H_MOTOR_
#define _H_MOTOR_

#include <array>

#include "pin.hpp"
#include "pwm.hpp"
#include "events.hpp"


namespace Motor {

class Driver {
public:

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

  Driver(TIM_TypeDef *timer, int pwm_frequency, const Pin &in1, const Pin &in2);
  void init(void) { _pwm.init(); }

  void stop(void);
  void brake(void);
  void forward(int pwm_duty_pct);
  void reverse(int pwm_duty_pct);

private:

  PWM _pwm;
};


#if 0
class Encoder {
public:

  static const int SAMPLE_COUNT = 32;

  enum Edge { RISING, FALLING };

  // We need to give a pin here, and we want to capture times between
  // edges, so we need a timer too.
  //
  // (Might also need some other timer configuration parameters.)
  //
  // Also need something to determine how many edge times to keep for
  // averaging?

  // int(pi * 60 * 1000) = 188495
  //
  // Motor::Encoder left_encoder(PB5, TIM9, 188495);

  Encoder(Pin &pulse, TIM_TypeDef *timer, int wheel_circumference_microns);

  // QUESTIONS:
  //
  //  - How will you want to use this? Returning a single interval
  //    gives you some sort of instantaneous measure of speed, but
  //    that might not be useful a lot of the time.
  //  - Define averaging for speed over a fixed time interval rather
  //    than a fixed set of intervals?
  //  - Might need to write some code using this to make the API
  //    concrete.
  //  - Start with an API oriented toward getting the hardware
  //    working: measure individual intervals between edges from the
  //    photoencoder, and once that's working, think about what might
  //    be needed in terms of speed calculations.

  int last_full_tooth_interval(void) const;
  int last_interval(void) const;
  Edge last_interval_edge(void) const;
  int speed_mm_per_s(void) const;

private:

  std::array<int, SAMPLE_COUNT> _edge_intervals;
  Edge _last_edge = FALLING;
};


class Torque : public Events::Consumer {
public:

  static const int SAMPLE_COUNT = 32;

  // Hmmm... ADC setup is quite complicated. What we want to do is
  // just to collect samples on a regular basis, probably based just
  // on SysTicks. It's not possible to run the ADC on the SysTick
  // timer though, so we might need to dispatch conversions manually.
  // That seems shonky, so let's look at other options.
  //
  // Also need calibration data for conversion from ADC counts to
  // torque values.

  Torque(Pin &pin, ADC_TypeDef *adc, int measure_interval);

  // To be called from ADC EOC IRQ.
  void end_of_conversion_irq(void);

  void dispatch(const Events::Event &e) override;

private:

  std::array<int, SAMPLE_COUNT> samples;
};


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
