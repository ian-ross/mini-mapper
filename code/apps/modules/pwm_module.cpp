#include <cctype>
#include <cstring>

#include "pwm_module.hpp"
#include "pwm.hpp"
#include "terminal.hpp"

using Terminal = TerminalT<USART>;
extern Terminal terminal;


static bool parse_duty(const char *s, int &duty, bool &pct);


PWMModule::PWMModule() :
  Shell::Module{"pwm"}, pwm{TIM10, PB8, 5000} { }


// The variables supported by the BlinkyModule shell module are
// pwm-duty (permitted values are bare numbers and strings of the form
// "XX%"), pwm-polarity (permitted values are "pos" and "neg") and
// pwm-state (permitted values are "off" and "on").

Shell::CommandResult
PWMModule::set_variable(const char *name, const char *value) {
  if (!strcmp(name, "pwm-state")) {
    if (!strcmp(value, "on")) {
      pwm.on();
      return Shell::COMMAND_OK;
    } else if (!strcmp(value, "off")) {
      pwm.off();
      return Shell::COMMAND_OK;
    }
    return Shell::INVALID_VALUE_FOR_VARIABLE;
  } else if (!strcmp(name, "pwm-polarity")) {
    if (!strcmp(value, "pos")) {
      pwm.set_inverted(false);
      return Shell::COMMAND_OK;
    } else if (!strcmp(value, "neg")) {
      pwm.set_inverted(true);
      return Shell::COMMAND_OK;
    }
    return Shell::INVALID_VALUE_FOR_VARIABLE;
  } else if (!strcmp(name, "pwm-duty")) {
    int val;
    bool pct = false;
    if (!strcmp(value, "max")) {
      val = pwm.reload_count() + 1;
    } else if (!parse_duty(value, val, pct))
      return Shell::INVALID_VALUE_FOR_VARIABLE;
    if (pct) {
      pwm.set_duty_pct(val);
    } else {
      pwm.set_duty(val);
    }
    return Shell::COMMAND_OK;
  }
  return Shell::SKIPPED;
}

Shell::CommandResult PWMModule::show_variable(const char *name) {
  if (!strcmp(name, "pwm-state")) {
    terminal.println(pwm.is_on() ? "on" : "off");
    return Shell::COMMAND_OK;
  } else if (!strcmp(name, "pwm-polarity")) {
    terminal.println(pwm.is_inverted() ? "neg" : "pos");
    return Shell::COMMAND_OK;
  } else if (!strcmp(name, "pwm-duty")) {
    int duty = pwm.duty();
    int duty_pct = pwm.duty_pct();
    terminal.print(duty);
    terminal.print(" (");
    terminal.print(duty_pct);
    terminal.println("%)");
    return Shell::COMMAND_OK;
  } else if (!strcmp(name, "pwm-reload")) {
    terminal.println(pwm.reload_count());
    return Shell::COMMAND_OK;
  } else {
    return Shell::SKIPPED;
  }
}


// The only command supported by the PWMModule module is "pwm". This
// takes a single "on"/"off" parameter.

Shell::CommandResult PWMModule::run_command
  (const char *cmd, int nargs, char *args[]) {
  if (strcmp(cmd, "pwm")) return Shell::SKIPPED;
  if (nargs != 1) return Shell::COMMAND_ERROR;

  if (!strcmp(args[0], "on")) {
    pwm.on();
    return Shell::COMMAND_OK;
  } else if (!strcmp(args[0], "off")) {
    pwm.off();
    return Shell::COMMAND_OK;
  }
  return Shell::INVALID_VALUE_FOR_VARIABLE;
}


// Parse duty cycle values from strings.

static bool parse_duty(const char *s, int &val, bool &pct) {
  for (val = 0; *s != '\0' && isdigit(*s); ++s) {
    val = val * 10 + *s - '0';
  }
  pct = *s == '%';
  return *s == '\0' || *s == '%';
}
