// Simple terminal demonstrator.

// Hardware includes: Nucleo board BSP and setup functions.
#include "bsp-nucleo.h"
#include "setup.hpp"

// Library includes.
#include "events.hpp"
#include "pin.hpp"
#include "usart.hpp"
#include "terminal.hpp"


inline void wfe(void) { __WFE(); }
Events::Manager ev(wfe);

using Terminal = TerminalT<USART>;

USART usart3(3, PD8, GPIO_AF_7, PD9, GPIO_AF_7, DMAChannel { 1, 3, 4 });

Terminal terminal(usart3);

extern "C" void USART3_IRQHandler(void) { usart3.rx_irq(); }
extern "C" void DMA1_Stream3_IRQHandler(void) { usart3.tx_dma_irq(); }
extern "C" void SysTick_Handler(void) { ev.post(Events::SYSTICK); }


// Write a message to terminal every second (measured by SysTick), and
// write additional messages echoing input lines collected by the
// terminal.

class TerminalDemo : public Events::Consumer {
public:

  TerminalDemo() : Events::Consumer("TerminalDemo") { }

  // Event dispatch...
  void dispatch(const Events::Event &e) {
    switch (e.tag) {
    case Events::SYSTICK:
      if (++tick_counter == 1000) {
        tick_counter = 0;
        write_tick_message(count++);
      }
      return;

    case Events::TERMINAL_LINE_RECEIVED:
      process_line((TerminalRXBuffer)e.param);
      return;

    default:
      return;
    }
  }

private:

  void write_tick_message(int c);
  void process_line(TerminalRXBuffer buff);

  int tick_counter = 0;
  int count = 0;
};

void TerminalDemo::write_tick_message(int c) {
  terminal.print("TICK: ");
  terminal.println(c);
}

void TerminalDemo::process_line(TerminalRXBuffer buff) {
  terminal.print("RX: ");
  terminal.println(terminal.buffer(buff));
  mgr->post(Events::TERMINAL_LINE_PROCESSED);
}

TerminalDemo terminal_demo;

int main(void)
{
  enable_caches();
  configure_clock();
  SysTick_Config(SystemCoreClock / 1000);
  terminal.set_interactive(true);

  ev += usart3;
  ev += terminal;
  ev += terminal_demo;
  ev.loop();
}
