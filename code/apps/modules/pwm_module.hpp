#ifndef _H_PWM_MODULE_
#define _H_PWM_MODULE_

#include "bsp-nucleo.h"

#include "pin.hpp"
#include "shell.hpp"
#include "pwm.hpp"


// Shell module to control PWM driver.

class PWMModule : public Shell::Module {
public:

  PWMModule();
  void init(void) override { pwm1.init(); pwm4.init(); }

  // Shell command and variable processing (from Shell::Module).
  Shell::CommandResult set_variable(const char *name, const char *value) override;
  Shell::CommandResult show_variable(const char *name) override;
  Shell::CommandResult run_command(const char *cmd, int nargs, char *args[]) override;

private:

  PWM pwm1;
  PWM pwm4;
};

#endif
