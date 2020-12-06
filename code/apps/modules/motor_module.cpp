#include <cctype>
#include <cstring>
#include <math.h>

#include "motor_module.hpp"
#include "motor-controller.hpp"
#include "terminal.hpp"

using Terminal = TerminalT<USART>;
extern Terminal terminal;


static bool parse_int(const char *s, int &val);
static void print_frac(float x);

MotorModule::MotorModule() :
  Shell::Module{"motor"}, Events::Consumer{"MotorModule"},
  // TODO: FIX THIS
  torque_cal{},
  torque_dma{2, 0, 0},
  torque{TIM4, ADC1, torque_dma, 100, torque_cal, PA4, PA5},
  wheel_info{
    .circumference = static_cast<uint32_t>(60.0 * M_PI * 1000.0),
    .encoder_gear_ratio = 48,
    .encoder_teeth = 12
  },
  encoder{TIM2, PB10, PB11, wheel_info},
  controller{Motor::LEFT, torque, encoder, TIM9, 30000, PE5, PE6} { }


// Event dispatching just uses SysTick events to report values from
// the torque measurement and position encoder.

void MotorModule::dispatch(const Events::Event &e) {
  if (e.tag != Events::SYSTICK)
    return;

  if (torque_display_interval > 0 &&
      ++torque_display_ticks == torque_display_interval) {
    torque_display_ticks = 0;
    terminal.print("TORQUE: ADC=");
    print_frac(torque.adc_count(Motor::Torque::SMOOTHED, Motor::LEFT));
    terminal.print("  =>  ");
    print_frac(1000 * torque.current(Motor::Torque::SMOOTHED, Motor::LEFT));
    terminal.println(" mA");
  }

  if (encoder_display_interval > 0 &&
      ++encoder_display_ticks == encoder_display_interval) {
    encoder_display_ticks = 0;
    torque_display_ticks = 0;
    terminal.print("ENCODER: INT=");
    print_frac(encoder.interval(Motor::Encoder::BOXCAR, Motor::LEFT));
    terminal.print(" us   SPEED=");
    print_frac(1000 * encoder.speed(Motor::Encoder::BOXCAR, Motor::LEFT));
    terminal.println(" mm/s");
  }
}

static void print_frac(float x) {
  terminal.print(static_cast<int>(x));
  terminal.print(".");
  int frac = static_cast<int>(x * 100);
  if (frac == 0) terminal.print("00");
  else if (frac < 10) {
    terminal.print("0");
    terminal.print(frac);
  } else
    terminal.print(frac);
}


Shell::CommandResult
MotorModule::set_variable(const char *name, const char *value) {
  int val;
  if (!strcmp(name, "torque-interval")) {
    if (!parse_int(value, val))
      return Shell::INVALID_VALUE_FOR_VARIABLE;
    torque_display_interval_var = val;
    return Shell::COMMAND_OK;
  } else if (!strcmp(name, "encoder-interval")) {
    if (!parse_int(value, val))
      return Shell::INVALID_VALUE_FOR_VARIABLE;
    encoder_display_interval_var = val;
    return Shell::COMMAND_OK;
  }
  return Shell::SKIPPED;
}

Shell::CommandResult MotorModule::show_variable(const char *name) {
  if (!strcmp(name, "torque-interval")) {
    terminal.println(torque_display_interval_var);
    return Shell::COMMAND_OK;
  } else if (!strcmp(name, "encoder-interval")) {
    terminal.println(encoder_display_interval_var);
    return Shell::COMMAND_OK;
  }
  return Shell::SKIPPED;
}


// Commands:
//  - stop
//  - forward <pwm-duty-pct>
//  - reverse <pwm-duty-pct>
//  - torque on|off
//  - encoder on|off

Shell::CommandResult MotorModule::run_command
  (const char *cmd, int nargs, char *args[]) {
  if (!strcmp(cmd, "stop") && nargs == 0) {
    controller.stop();
    terminal.println("STOPPED");
    return Shell::COMMAND_OK;
  } else if (!strcmp(cmd, "forward") && nargs == 1) {
    int val;
    if (!parse_int(args[0], val))
      return Shell::COMMAND_PARAMETER_ERROR;
    if (val > 100) val = 100;
    controller.forward(val);
    return Shell::COMMAND_OK;
  } else if (!strcmp(cmd, "reverse") && nargs == 1) {
    int val;
    if (!parse_int(args[0], val))
      return Shell::COMMAND_PARAMETER_ERROR;
    if (val > 100) val = 100;
    controller.reverse(val);
    return Shell::COMMAND_OK;
  } else if (!strcmp(cmd, "torque") && nargs == 1) {
    if (!strcmp(args[0], "on")) {
      torque_display_interval = torque_display_interval_var;
      torque_display_ticks = 0;
      controller.measure_torque(true);
      return Shell::COMMAND_OK;
    } else if (!strcmp(args[0], "off")) {
      torque_display_interval = -1;
      return Shell::COMMAND_OK;
    }
    return Shell::COMMAND_PARAMETER_ERROR;
  } else if (!strcmp(cmd, "encoder") && nargs == 1) {
    if (!strcmp(args[0], "on")) {
      encoder_display_interval = encoder_display_interval_var;
      encoder_display_ticks = 0;
      controller.measure_torque(false);
      return Shell::COMMAND_OK;
    } else if (!strcmp(args[0], "off")) {
      encoder_display_interval = -1;
      return Shell::COMMAND_OK;
    }
    return Shell::COMMAND_PARAMETER_ERROR;
  }
  return Shell::SKIPPED;
}


// Parse PWM duty cycle values from strings.

static bool parse_int(const char *s, int &val) {
  for (val = 0; *s != '\0' && isdigit(*s); ++s) {
    val = val * 10 + *s - '0';
  }
  return *s == '\0';
}
