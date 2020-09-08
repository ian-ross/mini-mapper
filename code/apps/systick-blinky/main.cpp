// Simple SysTick-driven blinky demonstrating event loop and GPIO
// pin-handling library code.

// Hardware includes: Nucleo board BSP and setup functions.
#include "bsp-nucleo.h"
#include "setup.hpp"

// Library includes.
#include "events.hpp"
#include "pin.hpp"


// Define an event manager using the ARM WFE instruction to wait for
// new events. This is set up this way to allow for mocking of the
// "wait for event" functionality for Linux-side testing.
//
// (The extra inline wfe function is needed here because __WFE is
// defined as a macro, and Events::Manager needs a function as a
// template parameter. The compiler generates a single inline WFE
// instruction in the object file output.)

inline void wfe(void) { __WFE(); }

Events::Manager ev(wfe);


// Post an event on each SysTick. This is the pattern I'll follow for
// more or less all interrupt handlers: service the hardware as
// quickly as possible (nothing to do here), then post an event. All
// "bottom half" handling will then be driven by event dispatch. This
// more or less guarantees that you get out of the interrupt handler
// promptly, and makes it easy to structure code to handle both
// low-level hardware events and higher-level application events.

extern "C" void SysTick_Handler(void) {
  ev.post(Events::SYSTICK);
}


// Toggle a GPIO pin every N SysTick ticks, driven by consuming events
// from event manager.

class TickerToggler : public Events::Consumer {
public:

  // Which pin are we toggling? How many SysTick ticks between
  // toggles?
  TickerToggler(Pin &p, int t) : pin{p}, toggle_ticks{t} { }

  // Event dispatch...
  bool dispatch(const Events::Event &e) {
    // Skip anything that's not a SysTick.
    if (e.tag != Events::SYSTICK)
      return false;

    if (++tick_counter == toggle_ticks) {
      // Toggle the pin and reset our counter every time we roll over.
      tick_counter = 0;
      pin.Toggle();
    }

    // Mark that we didn't consume the event, so other event consumers
    // can use it too.
    return false;
  }

private:

  Pin &pin;
  int toggle_ticks;
  int tick_counter = 0;
};


int main(void)
{
  // Common hardware setup code: enable I- and D-caches, configure
  // clocks (maxed out at 216 MHz, driven by 8 MHz HSE input on Nucleo
  // board), set SysTick frequency to 1 kHz.
  enable_caches();
  configure_clock();
  SysTick_Config(SystemCoreClock / 1000);

  // Configure LED1 pin: port and pin values come from the Nucleo BSP,
  // and the Pin abstraction allows us to set up the pin mode easily
  // (incidentally enabling the right AHB peripheral clock as well.)
  Pin LED1(LED1_PORT, LED1_PIN);
  LED1.Output(GPIO_SPEED_VERY_HIGH, GPIO_TYPE_PUSH_PULL, GPIO_PUPD_NONE);

  // Create an event consumer to toggle the LED, add it to the event
  // manager and run the event loop.
  TickerToggler ticker(LED1, 250);
  ev += ticker;
  ev.loop();
}
