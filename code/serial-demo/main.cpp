// TODO:
//
// - Split UART code from terminal handling
// - Line-based receive callback
// - Error handling

#include <stdint.h>

#include "bsp/nucleo.h"
#include "pin.hpp"
#include "setup.hpp"
#include "term.hpp"


// Interrupt handlers.
extern "C" void USART3_IRQHandler(void) { term.rx_irq(); }
extern "C" void DMA1_Stream3_IRQHandler(void) { term.tx_dma_irq(); }

int main(void)
{
  enable_caches();
  configure_clock();

  // Set SysTick frequency to 1 kHz.
  SysTick_Config(SystemCoreClock / 1000);

  // Initialise GPIO: LED1 pin as output.
  Pin LED1(LED1_PORT, LED1_PIN);
  LED1.Output(GPIO_SPEED_VERY_HIGH, GPIO_TYPE_PUSH_PULL, GPIO_PUPD_NONE);

  // Run event loop, toggling LED with terminal handling.
  term.print("> ");
  term.idle_flush();
  while (1) {
    LED1.Toggle();
    delay_ms(100, [](){ term.idle_flush(); });
  }
}
