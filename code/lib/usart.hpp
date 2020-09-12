#ifndef _H_USART_
#define _H_USART_

#include "bsp-generic.h"
#include "pin.hpp"
#include "events.hpp"
#include "dma.hpp"

static const int USART_TX_BUFSIZE = 128;

class USART : public Events::Consumer {
public:

  USART(uint8_t iusart,
        const Pin &tx, GPIOAF tx_af,
        const Pin &rx, GPIOAF rx_af,
        const DMAChannel &dma_chan);
  ~USART() { }

  USART(const USART&) = delete;
  USART(USART&&) = delete;
  USART& operator=(const USART&) = delete;
  USART& operator=(USART&&) = delete;

  // Buffer a single character for transmission.
  void tx(char c);

  // Interrupt handlers for character RX and completion of TX DMA.
  void rx_irq(void);
  void tx_dma_irq(void);

  // Transmission control: the `flush` method just sets a flag
  // indicating that a DMA transmission needs to be started. The DMA
  // transmission is started at the next SysTick from the `dispatch`
  // method.
  void flush(void) { need_flush = true; }
  bool dispatch(const Events::Event &e) override;

private:

  void start_tx_dma(void);

  // Hardware resources: USART peripheral and DMA stream.
  USART_TypeDef *usart;
  DMA_Stream_TypeDef *dma;

  // Dual transmit buffers to allow output to be accumulated while a
  // DMA transmission is running from the other buffer.
  char tx_buff1[USART_TX_BUFSIZE], tx_buff2[USART_TX_BUFSIZE];
  char *tx_buffs[2] = {tx_buff1, tx_buff2};
  int tx_buff_idx = 0;
  char *tx_buff = tx_buffs[0];
  int tx_size = 0;

  // Control flags.
  volatile bool tx_sending = false;
  volatile bool tx_error = false;
  volatile bool need_flush = false;
};

#endif
