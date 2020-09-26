#ifndef _H_BLINKY_MODULE_
#define _H_BLINKY_MODULE_

#include "bsp-nucleo.h"

#include "pin.hpp"
#include "events.hpp"
#include "shell.hpp"


// Shell module to control LED states based on shell commands.
//
// This class inherits from both Shell::Module and Events::Consumer
// because it processes shell commands (provided by the command shell
// infrastructure, which makes use of events related to USART input
// and output), but also needs to be able to consume events in its own
// right to be able to respond to SysTick events.

class BlinkyModule : public Shell::Module, public Events::Consumer {
public:

  // The STM32F767ZI Nucleo board has three user LEDs on it.
  static const int NLEDS = 3;

  // Each LED can be off, fully on, or blinking.
  enum State { OFF, ON, BLINK };

  BlinkyModule() :
    Shell::Module("blinky"), Events::Consumer("BlinkyModule") { }
  void init(void) override;

  // Event dispatch (from Events::Consumer).
  void dispatch(const Events::Event &e) override;

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

#endif
