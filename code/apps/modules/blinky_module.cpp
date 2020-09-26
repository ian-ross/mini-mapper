#include <cstring>

#include "blinky_module.hpp"
#include "terminal.hpp"

using Terminal = TerminalT<USART>;
extern Terminal terminal;


// Initialising the blinky shell module means setting each of the LED
// GPIOs to the correct state (output, PP, no pull-up/down resistor).

void BlinkyModule::init(void) {
  for (auto led : leds)
    led.Output(GPIO_SPEED_VERY_HIGH, GPIO_TYPE_PUSH_PULL, GPIO_PUPD_NONE);
}


// Event dispatching just uses SysTick events to switch LEDs in the
// BLINK state on and off with the appropriate delays. Each LED has an
// independent blink rate.

void BlinkyModule::dispatch(const Events::Event &e) {
  if (e.tag != Events::SYSTICK)
    return;

  for (int i = 0; i < NLEDS; ++i) {
    if (states[i] == BLINK) {
      if (++tick_count[i] >= delay_ticks[i]) {
        tick_count[i] = 0;
        leds[i].Toggle();
      }
    }
  }
}


// The variables supported by the BlinkyModule shell module are
// led-state-1, led-state-2 and led-state-3 (permitted values are
// "off", "on" and "blink") and led-delay-1, led-delay-2 and
// led-delay-3 (values are integer milliseconds).

Shell::CommandResult
BlinkyModule::set_variable(const char *name, const char *value) {
  if (!strncmp(name, "led-state-", 10)) {
    // Extract LED number and parse new state value.
    int led = name[10] - '1';
    if (led < 0 || led > 2) return Shell::SKIPPED;
    State state;
    if (!parse(value, state)) return Shell::INVALID_VALUE_FOR_VARIABLE;

    // Set the state and return success.
    set_state(led, state);
    return Shell::COMMAND_OK;
  } else if (!strncmp(name, "led-delay-", 10)) {
    // Extract LED number and parse new delay value.
    int led = name[10] - '1';
    if (led < 0 || led > 2) return Shell::SKIPPED;
    int delay;
    if (!Shell::parse(value, delay)) return Shell::INVALID_VALUE_FOR_VARIABLE;

    // Set the delay value and return success.
    delay_ticks[led] = delay;
    return Shell::COMMAND_OK;
  } else {
    return Shell::SKIPPED;
  }
}

Shell::CommandResult BlinkyModule::show_variable(const char *name) {
  if (!strncmp(name, "led-state-", 10)) {
    // Extract LED number.
    int led = name[10] - '1';
    if (led < 0 || led > 2) return Shell::SKIPPED;

    // Convert current state to string for display and return success.
    switch (states[led]) {
    case ON: terminal.println("on"); break;
    case OFF: terminal.println("off"); break;
    case BLINK: terminal.println("blink"); break;
    }
    return Shell::COMMAND_OK;
  } else if (!strncmp(name, "led-delay-", 10)) {
    // Extract LED number.
    int led = name[10] - '1';
    if (led < 0 || led > 2) return Shell::SKIPPED;

    // Print current delay value and return success.
    terminal.println(delay_ticks[led]);
    return Shell::COMMAND_OK;
  } else {
    return Shell::SKIPPED;
  }
}


// The only command supported by the BlinkyModule module is "led".
// This takes two parameters, the LED number (1-3) and the state
// ("off", "on" or "blink"). Using this command has an identical
// effect to setting the value of one of the led-state-N variables.

Shell::CommandResult BlinkyModule::run_command
  (const char *cmd, int nargs, char *args[]) {
  if (strcmp(cmd, "led")) return Shell::SKIPPED;
  if (nargs != 2) return Shell::COMMAND_ERROR;

  int led;
  if (!Shell::parse(args[0], led)) return Shell::COMMAND_ERROR;
  if (led < 1 || led > NLEDS) return Shell::COMMAND_ERROR;
  --led;

  State state;
  if (!parse(args[1], state)) return Shell::COMMAND_ERROR;

  set_state(led, state);
  return Shell::COMMAND_OK;
}


// Helper function to set LED states. For BLINK, the LED is initially
// switched off and the blink tick count is reset.

void BlinkyModule::set_state(int led, State state) {
  states[led] = state;
  switch (state) {
  case ON:
    leds[led].Set();
    break;
  case OFF:
    leds[led].Reset();
    break;
  case BLINK:
    leds[led].Reset();
    tick_count[led] = 0;
    break;
  }
}


// Parse state values from strings.

bool BlinkyModule::parse(const char *s, State &st) {
  if (!strcmp(s, "on")) { st = ON; return true; }
  if (!strcmp(s, "off")) { st = OFF; return true; }
  if (!strcmp(s, "blink")) { st = BLINK; return true; }
  return false;
}
