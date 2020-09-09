// Simple SysTick-driven USART demonstration.

// Hardware includes: Nucleo board BSP and setup functions.
#include "bsp-nucleo.h"
#include "setup.hpp"

// Library includes.
#include "events.hpp"
#include "pin.hpp"
#include "usart.hpp"


inline void wfe(void) { __WFE(); }
Events::Manager ev(wfe);


USART usart3(3, PD8, GPIO_AF_7, PD9, GPIO_AF_7, DMAChannel { 1, 3, 4 });

extern "C" void USART3_IRQHandler(void) { usart3.rx_irq(); }
extern "C" void DMA1_Stream3_IRQHandler(void) { usart3.tx_dma_irq(); }
extern "C" void SysTick_Handler(void) { ev.post(Events::SYSTICK); }


// Write a message to USART every second (measured by SysTick), and
// write additional messages echoing characters received on USART.

class TickerWriter : public Events::Consumer {
public:

  TickerWriter() { }

  // Event dispatch...
  bool dispatch(const Events::Event &e) {
    switch (e.tag) {
    case Events::SYSTICK:
      if (++tick_counter == 1000) {
        tick_counter = 0;
        write_tick_message(count++);
      }
      return false;

    case Events::USART_RX_CHAR:
      write_rx_message(e.param);
      return true;

    default:
      return false;
    }
  }

private:

  void write_tick_message(int c);
  void write_rx_message(char c);

  int tick_counter = 0;
  int count = 0;
};

const int MAX_DIGITS = 16;
char buff[MAX_DIGITS + 1];

void TickerWriter::write_tick_message(int c) {
  for (auto ch: "TICK: ") usart3.tx(ch);
  buff[MAX_DIGITS] = '\0';
  int idx = MAX_DIGITS;
  do {
    buff[--idx] = '0' + c % 10;
    c /= 10;
  } while (c > 0);
  while (buff[idx])
    usart3.tx(buff[idx++]);
  for (auto ch: "\r\n") usart3.tx(ch);
  usart3.flush();
}

void TickerWriter::write_rx_message(char c) {
  for (auto ch: "RX: '") usart3.tx(ch);
  usart3.tx(c);
  for (auto ch: "'\r\n") usart3.tx(ch);
  usart3.flush();
}

TickerWriter ticker;

int main(void)
{
  enable_caches();
  configure_clock();
  SysTick_Config(SystemCoreClock / 1000);

  ev += ticker;
  ev += usart3;
  ev.loop();
}
