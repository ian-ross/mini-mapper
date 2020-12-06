#ifndef _H_MOTOR_MODULE_
#define _H_MOTOR_MODULE_

#include "bsp-nucleo.h"

#include "pin.hpp"
#include "dma.hpp"
#include "shell.hpp"
#include "motor-torque.hpp"
#include "motor-encoder.hpp"
#include "motor-controller.hpp"


// Shell module to control MOTOR driver.

class MotorModule : public Shell::Module, public Events::Consumer {
public:

  MotorModule();
  void init(void) override { controller.init(); }

  // Event dispatch (from Events::Consumer).
  void dispatch(const Events::Event &e) override;

  // Shell command and variable processing (from Shell::Module).
  Shell::CommandResult set_variable(const char *name, const char *value) override;
  Shell::CommandResult show_variable(const char *name) override;
  Shell::CommandResult run_command(const char *cmd, int nargs, char *args[]) override;

  // Called from ISR for torque DMA stream.
  void torque_dma_irq(void) { torque.dma_transfer_complete_irq(); }

  // Called from ISR for encoder timer capture.
  void encoder_irq(void) { encoder.timer_capture_irq(); }

  Motor::Encoder &motor_encoder(void) { return encoder; }

private:

  Motor::Torque::Calibration torque_cal;
  DMAChannel torque_dma;
  Motor::Torque torque;
  Motor::Encoder::WheelInfo wheel_info;
  Motor::Encoder encoder;
  Motor::Controller controller;

  int torque_display_ticks;
  int torque_display_interval = -1;
  int torque_display_interval_var = 1000;
  int encoder_display_ticks;
  int encoder_display_interval = -1;
  int encoder_display_interval_var = 1000;
};

#endif
