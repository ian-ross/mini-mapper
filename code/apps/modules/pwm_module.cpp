#include <cctype>
#include <cstring>

#include "pwm_module.hpp"
#include "pwm.hpp"
#include "terminal.hpp"

using Terminal = TerminalT<USART>;
extern Terminal terminal;


static bool parse_duty(const char *s, int &duty, bool &pct);
static bool parse_channel(const char *s, PWM::Channel *ch);


PWMModule::PWMModule() :
  Shell::Module{"pwm"},
  pwm1{TIM10, 5000, PB8},
  pwm4{TIM3, 5000, PA6, PA7, PC8, PC9} { }


// The variables supported by the BlinkyModule shell module are duty
// (permitted values are bare numbers and strings of the form "XX%"),
// polarity (permitted values are "pos" and "neg") and state
// (permitted values are "off" and "on"). There are state variables
// for each PWM instance (pwm1-state, pwm4-state) and polarity and
// duty variables for each channel of each PWM instance (pwm1-duty,
// pwm4-duty-{1|2|3|4}, pwm1-polarity, pwm4-polarity-{1|2|3|4}).

Shell::CommandResult
PWMModule::set_variable(const char *name, const char *value) {
  PWM *pwm = nullptr;
  if (!strncmp(name, "pwm1", 4)) pwm = &pwm1;
  if (!strncmp(name, "pwm4", 4)) pwm = &pwm4;
  if (pwm == nullptr) return Shell::SKIPPED;
  const char *subname = name + 4;

  if (!strcmp(subname, "-state")) {
    if (!strcmp(value, "on")) {
      pwm->on();
      return Shell::COMMAND_OK;
    } else if (!strcmp(value, "off")) {
      pwm->off();
      return Shell::COMMAND_OK;
    }
    return Shell::INVALID_VALUE_FOR_VARIABLE;
  } else if (!strncmp(subname, "-polarity", 9)) {
    PWM::Channel ch;
    if (!parse_channel(subname + 9, &ch))
      return Shell::SKIPPED;
    if (!strcmp(value, "pos")) {
      pwm->set_inverted(ch, false);
      return Shell::COMMAND_OK;
    } else if (!strcmp(value, "neg")) {
      pwm->set_inverted(ch, true);
      return Shell::COMMAND_OK;
    }
    return Shell::INVALID_VALUE_FOR_VARIABLE;
  } else if (!strncmp(subname, "-duty", 5)) {
    PWM::Channel ch;
    if (!parse_channel(subname + 5, &ch))
      return Shell::SKIPPED;
    int val;
    bool pct = false;
    if (!strcmp(value, "max")) {
      val = pwm->reload_count() + 1;
    } else if (!parse_duty(value, val, pct))
      return Shell::INVALID_VALUE_FOR_VARIABLE;
    if (pct) {
      pwm->set_duty_pct(ch, val);
    } else {
      pwm->set_duty(ch, val);
    }
    return Shell::COMMAND_OK;
  }
  return Shell::SKIPPED;
}

Shell::CommandResult PWMModule::show_variable(const char *name) {
  PWM *pwm = nullptr;
  if (!strncmp(name, "pwm1", 4)) pwm = &pwm1;
  if (!strncmp(name, "pwm4", 4)) pwm = &pwm4;
  if (pwm == nullptr) return Shell::SKIPPED;
  const char *subname = name + 4;

  if (!strcmp(subname, "-state")) {
    terminal.println(pwm->is_on() ? "on" : "off");
    return Shell::COMMAND_OK;
  } else if (!strncmp(subname, "-polarity", 9)) {
    PWM::Channel ch;
    if (!parse_channel(subname + 9, &ch))
      return Shell::SKIPPED;
    terminal.println(pwm->is_inverted(ch) ? "neg" : "pos");
    return Shell::COMMAND_OK;
  } else if (!strncmp(subname, "-duty", 5)) {
    PWM::Channel ch;
    if (!parse_channel(subname + 5, &ch))
      return Shell::SKIPPED;
    int duty = pwm->duty(ch);
    int duty_pct = pwm->duty_pct(ch);
    terminal.print(duty);
    terminal.print(" (");
    terminal.print(duty_pct);
    terminal.println("%)");
    return Shell::COMMAND_OK;
  } else if (!strcmp(subname, "-reload")) {
    terminal.println(pwm->reload_count());
    return Shell::COMMAND_OK;
  } else {
    return Shell::SKIPPED;
  }
}


// The only commands supported by the PWMModule module are "pwm1" and
// "pwm4". These both take a single "on"/"off" parameter.

Shell::CommandResult PWMModule::run_command
  (const char *cmd, int nargs, char *args[]) {
  PWM *pwm = nullptr;
  if (!strcmp(cmd, "pwm1")) pwm = &pwm1;
  if (!strcmp(cmd, "pwm4")) pwm = &pwm4;
  if (pwm == nullptr)
    return Shell::SKIPPED;
  if (nargs != 1) return Shell::COMMAND_ERROR;

  if (!strcmp(args[0], "on")) {
    pwm->on();
    return Shell::COMMAND_OK;
  } else if (!strcmp(args[0], "off")) {
    pwm->off();
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


// Parse PWM channel values from strings.

static bool parse_channel(const char *s, PWM::Channel *ch) {
  if (s[0] == '\0') {
    *ch = PWM::CH1;
    return true;
  } else if (s[1] == '\0') {
    switch (s[0]) {
    case '1': *ch = PWM::CH1; return true;
    case '2': *ch = PWM::CH2; return true;
    case '3': *ch = PWM::CH3; return true;
    case '4': *ch = PWM::CH4; return true;
    default:
      break;
    }
  }
  return false;
}
