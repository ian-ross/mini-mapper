// Simple SysTick-driven "multi-blinky" running from command shell.

#include <cstring>

// Hardware includes: Nucleo board BSP and setup functions.
#include "bsp-nucleo.h"
#include "setup.hpp"

// Library includes.
#include "events.hpp"
#include "pin.hpp"
#include "usart.hpp"
#include "terminal.hpp"
#include "shell.hpp"


// Event management for real hardware plus setup for terminal using
// USART3 (which is connected to the ST-Link debugger on the
// STM32F767ZI Nucleo board).

inline void wfe(void) { __WFE(); }
Events::Manager ev(wfe);

USART usart3(3, PD8, GPIO_AF_7, PD9, GPIO_AF_7, DMAChannel { 1, 3, 4 });
Terminal terminal(usart3);

extern "C" void USART3_IRQHandler(void) { usart3.rx_irq(); }
extern "C" void DMA1_Stream3_IRQHandler(void) { usart3.tx_dma_irq(); }
extern "C" void SysTick_Handler(void) { ev.post(Events::SYSTICK); }


// Shell module to control LED states based on shell commands.
//
// This class inherits from both Shell::Module and Events::Consumer
// because it processes shell commands (provided by the command shell
// infrastructure, which makes use of events related to USART input
// and output), but also needs to be able to consume events in its own
// right to be able to respond to SysTick events.

class Blinky : public Shell::Module, public Events::Consumer {
public:

  // The STM32F767ZI Nucleo board has three user LEDs on it.
  static const int NLEDS = 3;

  // Each LED can be off, fully on, or blinking.
  enum State { OFF, ON, BLINK };

  Blinky();

  // Event dispatch (from Events::Consumer).
  bool dispatch(const Events::Event &e) override;

  // Shell command and variable processing (from Shell::Module).
  Shell::CommandResult set_variable(const char *name, const char *value) override;
  Shell::CommandResult show_variable(const char *name) override;
  Shell::CommandResult run_command(const char *cmd, int nargs, char *args[]) override;

private:

  // Parse a state from a string.
  bool parse(const char *s, State &st);

  // Set up LED states.
  void set_state(int led, State state);

  // LED GPIO pins.
  std::array<Pin, NLEDS> leds {
    Pin{LED1_PORT, LED1_PIN},
    Pin{LED2_PORT, LED2_PIN},
    Pin{LED3_PORT, LED3_PIN}
  };

  // Current state of each LED.
  std::array<State, NLEDS> states { OFF, OFF, OFF };

  // Current SysTick tick count for each LED, used for blinking.
  std::array<int, NLEDS> tick_count { 0, 0, 0 };

  // On/off blink delay for each LED in milliseconds.
  std::array<int, NLEDS> delay_ticks { 250, 250, 250 };
};


// Initialising the blinky shell module basically means setting up the
// names used for debugging and setting each of the LED GPIOs to the
// correct state (output, PP, no pull-up/down resistor).

Blinky::Blinky() : Shell::Module("blinky"), Events::Consumer("Blinky") {
  for (auto led : leds)
    led.Output(GPIO_SPEED_VERY_HIGH, GPIO_TYPE_PUSH_PULL, GPIO_PUPD_NONE);
}


// Event dispatching just uses SysTick events to switch LEDs in the
// BLINK state on and off with the appropriate delays. Each LED has an
// independent blink rate.

bool Blinky::dispatch(const Events::Event &e) {
  if (e.tag != Events::SYSTICK)
    return false;

  for (int i = 0; i < NLEDS; ++i) {
    if (states[i] == BLINK) {
      if (++tick_count[i] >= delay_ticks[i]) {
        tick_count[i] = 0;
        leds[i].Toggle();
      }
    }
  }

  return false;
}


// The variables supported by the Blinky shell module are led-state-1,
// led-state-2 and led-state-3 (permitted values are "off", "on" and
// "blink") and led-delay-1, led-delay-2 and led-delay-3 (values are
// integer milliseconds).

Shell::CommandResult Blinky::set_variable(const char *name, const char *value) {
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

Shell::CommandResult Blinky::show_variable(const char *name) {
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


// The only command supported by the Blinky module is "led". This
// takes two parameters, the LED number (1-3) and the state ("off",
// "on" or "blink"). Using this command has an identical effect to
// setting the value of one of the led-state-N variables.

Shell::CommandResult Blinky::run_command(const char *cmd, int nargs,
                                         char *args[]) {
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

void Blinky::set_state(int led, State state) {
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

bool Blinky::parse(const char *s, State &st) {
  if (!strcmp(s, "on")) { st = ON; return true; }
  if (!strcmp(s, "off")) { st = OFF; return true; }
  if (!strcmp(s, "blink")) { st = BLINK; return true; }
  return false;
}


Blinky blinky;

int main(void)
{
  // Standard cache and clock setup, set SysTick to 1 kHz, set
  // terminal to interactive (which gives prompts and rudimentary line
  // editing).
  enable_caches();
  configure_clock();
  SysTick_Config(SystemCoreClock / 1000);
  terminal.set_interactive(true);

  // Create command shell with core module (which implements the "set"
  // and "show" commands) and LED blinky module.
  Shell::CommandShell shell(terminal);
  shell += blinky;

  // Add all event consumers to event manager.
  ev += usart3;
  ev += terminal;
  ev += shell;
  ev += blinky;

  ev.loop();
}
