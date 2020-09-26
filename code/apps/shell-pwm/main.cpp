// Command shell with PWM and LED blinky modules.

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

// Shell modules.
#include "blinky_module.hpp"
#include "pwm_module.hpp"


// Event management for real hardware plus setup for terminal using
// USART3 (which is connected to the ST-Link debugger on the
// STM32F767ZI Nucleo board).

inline void wfe(void) { __WFE(); }
Events::Manager ev(wfe);

using Terminal = TerminalT<USART>;
USART usart3(3, PD8, GPIO_AF_7, PD9, GPIO_AF_7, DMAChannel { 1, 3, 4 });
Terminal terminal(usart3);

extern "C" void USART3_IRQHandler(void) { usart3.rx_irq(); }
extern "C" void DMA1_Stream3_IRQHandler(void) { usart3.tx_dma_irq(); }
extern "C" void SysTick_Handler(void) { ev.post(Events::SYSTICK); }



BlinkyModule blinky;
PWMModule pwm;

int main(void)
{
  // Standard cache and clock setup, set SysTick to 1 kHz, set
  // terminal to interactive (which gives prompts and rudimentary line
  // editing).
  enable_caches();
  configure_clock();
  SysTick_Config(SystemCoreClock / 1000);
  terminal.set_interactive(true);
  pwm.init();

  // Create command shell with core module (which implements the "set"
  // and "show" commands) and PWM and LED blinky modules.
  Shell::CommandShell shell(terminal);
  shell += pwm;
  shell += blinky;

  // Add all event consumers to event manager.
  ev += usart3;
  ev += terminal;
  ev += shell;
  ev += blinky;

  terminal.print("ARR = ");
  terminal.println(pwm.reload());
  ev.loop();
}
