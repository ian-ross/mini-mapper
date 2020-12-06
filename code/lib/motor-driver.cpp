#include <numeric>

#include "motor-driver.hpp"


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
  (TIM_TypeDef *timer, uint32_t pwm_freq,
   const Pin &in1, const Pin &in2) :
    _pwm{timer, pwm_freq, in1, in2} {}

void Motor::Driver::init(void) {
  _pwm.init();
  _pwm.on();
  _pwm.set_inverted(PWM::CH1, true);
  _pwm.set_inverted(PWM::CH2, true);
}

void Motor::Driver::stop(void) {
  _pwm.set(PWM::CH1);
  _pwm.set(PWM::CH2);
}

void Motor::Driver::brake(void) {
  _pwm.reset(PWM::CH1);
  _pwm.reset(PWM::CH2);
}

void Motor::Driver::forward(int pwm_duty_pct) {
  _pwm.set(PWM::CH1);
  _pwm.set_duty_pct(PWM::CH2, pwm_duty_pct);
}

void Motor::Driver::reverse(int pwm_duty_pct) {
  _pwm.set_duty_pct(PWM::CH1, pwm_duty_pct);
  _pwm.set(PWM::CH2);
}


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
