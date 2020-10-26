#include "motor.hpp"


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

Motor::Driver::Driver
  (TIM_TypeDef *timer, int pwm_freq, const Pin &in1, const Pin &in2) :
    _pwm(timer, pwm_freq, in1, in2) {}

void Motor::Driver::init(void) {
  _pwm.init();
  _pwm.set_inverted(PWM::CH1, true);
  _pwm.set_inverted(PWM::CH2, true);
}

void Motor::Driver::stop(void) {
  _pwm.reset(PWM::CH1);
  _pwm.reset(PWM::CH2);
}

void Motor::Driver::brake(void) {
  _pwm.set(PWM::CH1);
  _pwm.set(PWM::CH2);
}

void Motor::Driver::forward(int pwm_duty_pct) {
  _pwm.set(PWM::CH1);
  _pwm.set_duty_pct(PWM::CH2, pwm_duty_pct);
}

void Motor::Driver::reverse(int pwm_duty_pct) {
  _pwm.set_duty_pct(PWM::CH1, pwm_duty_pct);
  _pwm.set(PWM::CH2);
}


#if 0
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

Motor::Encoder::Encoder(Pin &pulse, TIM_TypeDef *timer,
                        int wheel_circumference_microns) {}

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

int Motor::Encoder::last_full_tooth_interval(void) const {
  return 0;
}

int Motor::Encoder::last_interval(void) const { return 0; }

Motor::Encoder::Edge Motor::Encoder::last_interval_edge(void) const {
  return RISING;
}

int Motor::Encoder::speed_mm_per_s(void) const { return 0; }



// Hmmm... ADC setup is quite complicated. What we want to do is
// just to collect samples on a regular basis, probably based just
// on SysTicks. It's not possible to run the ADC on the SysTick
// timer though, so we might need to dispatch conversions manually.
// That seems shonky, so let's look at other options.
//
// Also need calibration data for conversion from ADC counts to
// torque values.

Motor::Torque::Torque(Pin &pin, ADC_TypeDef *adc, int measure_interval) {}

// To be called from ADC EOC IRQ.
void Motor::Torque::end_of_conversion_irq(void) {}

void Motor::Torque::dispatch(const Events::Event &e) {}



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

extern char fatal_msg[];

// *** In the motor driver, both PWM channels are inverted, so zero
// *** duty means HIGH, max duty means LOW.

TEST_CASE("Motor::Driver initialisation") {
  init_mock_mcu();
  fatal_msg[0] = '\0';

  SUBCASE("create and initialise driver with valid PWM timer and pins") {
    Motor::Driver driver(TIM9, 30000, PA2, PA3);
    driver.init();
    CHECK(strcmp(fatal_msg, "") == 0);
    // Check that PWM mode is set properly for the timer.
    CHECK(((TIM9->CCMR1 & TIM_CCMR1_OC1M_Msk) >> TIM_CCMR1_OC1M_Pos) == 6);
    CHECK(driver._pwm.is_inverted(PWM::CH1));
    CHECK(driver._pwm.is_inverted(PWM::CH2));
  }

  SUBCASE("create and initialise driver with invalid PWM timer and pins") {
    Motor::Driver driver(TIM9, 30000, PA1, PA3);
    driver.init();
    CHECK(strcmp(fatal_msg, "") != 0);
  }
}

TEST_CASE("Motor::Driver run") {
  Motor::Driver driver(TIM9, 30000, PA2, PA3);
  driver.init();

  SUBCASE("run forward 100% => IN1 high, IN2 low") {
    driver.forward(100);
    CHECK(driver._pwm.duty(PWM::CH1) == 0);
    CHECK(driver._pwm.duty(PWM::CH2) == driver._pwm.reload_count());
  }

  SUBCASE("run backward 100% => IN2 high, IN1 low") {
    driver.reverse(100);
    CHECK(driver._pwm.duty(PWM::CH2) == 0);
    CHECK(driver._pwm.duty(PWM::CH1) == driver._pwm.reload_count());
  }

  SUBCASE("run forward 50% => IN1 high, IN2 50%") {
    driver.forward(50);
    CHECK(driver._pwm.duty(PWM::CH1) == 0);
    CHECK(driver._pwm.duty(PWM::CH2) == driver._pwm.reload_count() / 2);
  }

  SUBCASE("run backward 50% => IN2 high, IN1 50%") {
    driver.reverse(50);
    CHECK(driver._pwm.duty(PWM::CH2) == 0);
    CHECK(driver._pwm.duty(PWM::CH1) == driver._pwm.reload_count() / 2);
  }

  SUBCASE("run forward, then stop => IN1 low, IN2 low") {
    driver.forward(100);
    driver.stop();
    CHECK(driver._pwm.duty(PWM::CH1) == driver._pwm.reload_count() + 1);
    CHECK(driver._pwm.duty(PWM::CH2) == driver._pwm.reload_count() + 1);
  }

  SUBCASE("run forward, then brake => IN1 high, IN2 high") {
    driver.forward(100);
    driver.brake();
    CHECK(driver._pwm.duty(PWM::CH1) == 0);
    CHECK(driver._pwm.duty(PWM::CH2) == 0);
  }
}

#endif
