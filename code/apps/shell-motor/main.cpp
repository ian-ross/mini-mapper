// Command shell with motor and LED blinky modules.

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
#include "motor_module.hpp"


// Event management for real hardware plus setup for terminal using
// USART3 (which is connected to the ST-Link debugger on the
// STM32F767ZI Nucleo board).

inline void wfe(void) { __WFE(); }
Events::Manager ev(wfe);

using Terminal = TerminalT<USART>;
USART usart3(3, PD8, GPIO_AF_7, PD9, GPIO_AF_7, DMAChannel { 1, 3, 4 });
Terminal terminal(usart3);

BlinkyModule blinky;
MotorModule motor;

extern "C" void USART3_IRQHandler(void) { usart3.rx_irq(); }
extern "C" void DMA1_Stream3_IRQHandler(void) { usart3.tx_dma_irq(); }
extern "C" void SysTick_Handler(void) { ev.post(Events::SYSTICK); }
extern "C" void DMA2_Stream0_IRQHandler(void) { motor.torque_dma_irq(); }


int main(void)
{
  // Standard cache and clock setup, set SysTick to 1 kHz, set
  // terminal to interactive (which gives prompts and rudimentary line
  // editing).
  enable_caches();
  configure_clock();
  SysTick_Config(SystemCoreClock / 1000);
  terminal.set_interactive(true);
  motor.init();

  // Create command shell with core module (which implements the "set"
  // and "show" commands) and motor and LED blinky modules.
  Shell::CommandShell shell(terminal);
  shell += motor;
  shell += blinky;

  // Add all event consumers to event manager.
  ev += usart3;
  ev += terminal;
  ev += shell;
  ev += blinky;
  ev += motor;
  // ev += motor.encoder();

  ev.loop();
}
