#include "motor-controller.hpp"

Motor::Controller::Controller
  (Instance instance, Torque &torque, Encoder &encoder,
   TIM_TypeDef *pwm_timer, uint32_t pwm_frequency,
   const Pin &pwm_pin1, const Pin &pwm_pin2)
  : _instance{instance}, _torque{torque}, _encoder{encoder},
    _driver{pwm_timer, pwm_frequency, pwm_pin1, pwm_pin2} {}

void Motor::Controller::init(void) {
  _torque.init();
  _encoder.init();
  _driver.init();
}

void Motor::Controller::measure_torque(bool onoff) {
  if (onoff)
    _torque.start();
  else
    _torque.stop();
}

//----------------------------------------------------------------------
//
//  TESTS
//

#ifdef TEST

#include <cmath>
#include <iostream>
#include "doctest.h"
#include "doctest/trompeloeil.hpp"
#include "events_mock.hpp"

extern char fatal_msg[];

TEST_CASE("Motor::Controller initialisation") {
  init_mock_mcu();
  fatal_msg[0] = '\0';

  SUBCASE("create and initialise valid controller") {
    Motor::Torque::Calibration cal;
    DMAChannel dma{2, 0, 0};
    Motor::Torque torque{TIM6, ADC1, dma, 100 /* ms */, cal, PA4, PA5};

    Motor::Encoder::WheelInfo wheel_info{
      .circumference = static_cast<uint32_t>(60.0 * M_PI * 1000.0),
      .encoder_gear_ratio = 48,
      .encoder_teeth = 12
    };
    Motor::Encoder encoder(TIM2, PB10, PB11, wheel_info);

    Motor::Controller controller
      {Motor::LEFT, torque, encoder, TIM9, 30000, PE5, PE6};
    controller.init();
  }
}


#endif
