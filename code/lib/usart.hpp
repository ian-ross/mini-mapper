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
        const DMAChannel &dma_chan) :
    Events::Consumer("USART"), _iusart(iusart),
    _tx(tx), _tx_af(tx_af),
    _rx(rx), _rx_af(rx_af),
    _dma_chan(dma_chan),
    _usart(usart_base(iusart)),
    _dma(usart_dma_stream(iusart)) { }
  ~USART() { }

  USART(const USART&) = delete;
  USART(USART&&) = delete;
  USART& operator=(const USART&) = delete;
  USART& operator=(USART&&) = delete;

  int index(void) const { return _iusart; }

  // Buffer a single character for transmission.
  void tx(char c);

  // Interrupt handlers for character RX and completion of TX DMA.
  void rx_irq(void);
  void tx_dma_irq(void);

  // Transmission control: the `flush` method just sets a flag
  // indicating that a DMA transmission needs to be started. The DMA
  // transmission is started at the next SysTick from the `dispatch`
  // method.
  void flush(void) { _need_flush = true; }
  void dispatch(const Events::Event &e) override;

private:

  uint8_t _iusart;
  const Pin &_tx;
  GPIOAF _tx_af;
  const Pin &_rx;
  GPIOAF _rx_af;
  const DMAChannel _dma_chan;

  void init(void);
  void start_tx_dma(void);

  // Hardware resources: USART peripheral and DMA stream.
  USART_TypeDef *_usart;
  DMA_Stream_TypeDef *_dma;

  // Dual transmit buffers to allow output to be accumulated while a
  // DMA transmission is running from the other buffer.
  char _tx_buff1[USART_TX_BUFSIZE], _tx_buff2[USART_TX_BUFSIZE];
  char *_tx_buffs[2] = {_tx_buff1, _tx_buff2};
  int _tx_buff_idx = 0;
  char *_tx_buff = _tx_buffs[0];
  int _tx_size = 0;

  // Control flags.
  volatile bool _tx_sending = false;
  volatile bool _tx_error = false;
  volatile bool _need_flush = false;

  static USART_TypeDef *usart_base(int iusart);
  static DMA_Stream_TypeDef *usart_dma_stream(int iusart);
};

#endif
