#include "dma.hpp"
#include "events.hpp"
#include "usart.hpp"

static USART_TypeDef *usart_base(int iusart);
static DMA_Stream_TypeDef *usart_dma_stream(int iusart);
static IRQn_Type usart_irqn(int iusart);


// Set up USART for interrupt-driven RX, DMA-driven TX.

void USART::init(void) {
  volatile uint32_t &apbenr_reg = RCC->APB1ENR;
  const uint32_t apbenr_bit = RCC_APB1ENR_USART3EN;
  volatile uint32_t &ahbenr_dma_reg = RCC->AHB1ENR;
  const uint32_t ahbenr_dma_bit = RCC_AHB1ENR_DMA1EN + _dma_chan.dma - 1;

  // Disable USART.
  CLEAR_BIT(_usart->CR1, USART_CR1_UE);

  // Enable USART APB clock.
  SET_BIT(apbenr_reg, apbenr_bit);
  (void)READ_BIT(apbenr_reg, apbenr_bit);

  // Select system clock (216 MHz) as clock source for USART.
  const uint32_t dckcfgr2_pos = 2 * (_iusart - 1);
  const uint32_t dckcfgr2_mask = 0x03 << dckcfgr2_pos;
  MODIFY_REG(RCC->DCKCFGR2, dckcfgr2_mask, 0x01 << dckcfgr2_pos);

  // Set word length (M1 = 0, M0 = 0 => 1 start bit, 8 data bits, n
  // stop bits).
  CLEAR_BIT(_usart->CR1, USART_CR1_M0);
  CLEAR_BIT(_usart->CR1, USART_CR1_M1);

  // Disable parity.
  CLEAR_BIT(_usart->CR1, USART_CR1_PCE);

  // Disable auto baudrate detection.
  CLEAR_BIT(_usart->CR2, USART_CR2_ABREN);

  // Send/receive LSB first.
  CLEAR_BIT(_usart->CR2, USART_CR2_MSBFIRST);

  // Set oversampling rate to 16 and select baud rate 115200 based on
  // USART clock running at 216 MHz (value taken from Table 220 in
  // STM32F767ZI reference manual). (216000000 / 115200 = 0x0753)
  CLEAR_BIT(_usart->CR1, USART_CR1_OVER8);
  _usart->BRR = 0x0753;

  // One stop bit (USART.CR2.STOP[1:0] = 0 => 1 stop bit).
  MODIFY_REG(_usart->CR2, USART_CR2_STOP_Msk, 0);

  // Enable USART, receiver and transmitter.
  SET_BIT(_usart->CR1, USART_CR1_UE);
  SET_BIT(_usart->CR1, USART_CR1_RE);
  SET_BIT(_usart->CR1, USART_CR1_TE);

  // Pin configuration: set TX to output (PP, pull-up), RX to input,
  // set both to the appropriate alternate function.
  _tx.output(GPIO_SPEED_VERY_HIGH, GPIO_TYPE_PUSH_PULL, GPIO_PUPD_PULL_UP);
  _rx.input(GPIO_SPEED_VERY_HIGH);
  _tx.alternate(_tx_af);
  _rx.alternate(_rx_af);

  // Receive interrupt setup: enable USART interrupts in NVIC and
  // enable RX interrupt (also handles overrun errors).
  NVIC_EnableIRQ(usart_irqn(_iusart));
  SET_BIT(_usart->CR1, USART_CR1_RXNEIE);

  // Transmit DMA setup. (Following procedure in Section 8.3.18 of
  // reference manual.)

  // TODO: SOME OF THIS IS STILL SPECIFIC TO USART3, I THINK.

  // Enable DMA transmitter for UART.
  SET_BIT(_usart->CR3, USART_CR3_DMAT);

  // Enable clock for DMA1.
  SET_BIT(ahbenr_dma_reg, ahbenr_dma_bit);

  // 2. Set the peripheral port register address in the DMA_SxPAR
  //    register.
  _dma->PAR = (uintptr_t)&_usart->TDR;

  // 5. Select the DMA channel (request) using CHSEL[3:0] in the
  //    DMA_SxCR register.
  MODIFY_REG(_dma->CR, DMA_SxCR_CHSEL_Msk,
             _dma_chan.channel << DMA_SxCR_CHSEL_Pos);

  // DMA is flow controller.
  CLEAR_BIT(_dma->CR, DMA_SxCR_PFCTRL);

  // 7. Configure the stream priority using the PL[1:0] bits in the
  //    DMA_SxCR register.

  // Medium priority.
  MODIFY_REG(_dma->CR, DMA_SxCR_PL_Msk, 0x02 << DMA_SxCR_PL_Pos);

  // 8. Configure the FIFO usage (enable or disable, threshold in
  //    transmission and reception).
  CLEAR_BIT(_dma->FCR, DMA_SxFCR_DMDIS);            // No FIFO: direct mode.

  // 9. Configure the data transfer direction, peripheral and memory
  //    incremented/fixed mode, single or burst transactions,
  //    peripheral and memory data widths, circular mode,
  //    double-buffer mode and interrupts after half and/or full
  //    transfer, and/or errors in the DMA_SxCR register.

  // Memory-to-peripheral.
  MODIFY_REG(_dma->CR, DMA_SxCR_DIR_Msk, 0x01 << DMA_SxCR_DIR_Pos);

  // Increment memory, no increment peripheral.
  SET_BIT(_dma->CR, DMA_SxCR_MINC);
  CLEAR_BIT(_dma->CR, DMA_SxCR_PINC);

  // No burst at either end.
  MODIFY_REG(_dma->CR, DMA_SxCR_MBURST_Msk, 0x00 << DMA_SxCR_MBURST_Pos);
  MODIFY_REG(_dma->CR, DMA_SxCR_PBURST_Msk, 0x00 << DMA_SxCR_PBURST_Pos);

  // Byte size at both ends.
  MODIFY_REG(_dma->CR, DMA_SxCR_MSIZE_Msk, 0x00 << DMA_SxCR_MSIZE_Pos);
  MODIFY_REG(_dma->CR, DMA_SxCR_PSIZE_Msk, 0x00 << DMA_SxCR_PSIZE_Pos);

  // No circular mode, no double buffering.
  CLEAR_BIT(_dma->CR, DMA_SxCR_CIRC);
  CLEAR_BIT(_dma->CR, DMA_SxCR_DBM);

  // Transfer complete interrupt, no half-transfer interrupt, transfer
  // error interrupt and direct mode error interrupt.
  SET_BIT(_dma->CR, DMA_SxCR_TCIE);
  CLEAR_BIT(_dma->CR, DMA_SxCR_HTIE);
  SET_BIT(_dma->CR, DMA_SxCR_TEIE);
  SET_BIT(_dma->CR, DMA_SxCR_DMEIE);

  // Transmit DMA interrupt setup: enable DMA interrupts for DMA
  // channel attached to USART.
  NVIC_EnableIRQ(dma_irqn(_dma_chan));
}

// Buffer a single character for transmission.

void USART::tx(char c) {
  // Buffer overflow: clear buffer, post error.
  if (_tx_size >= USART_TX_BUFSIZE) {
    _tx_size = 0;
    if (mgr) mgr->post(Events::USART_TX_OVERFLOW);
    return;
  }

  // Buffer character.
  _tx_buff[_tx_size++] = c;
}


// Event handler: this checks whether we need to fire off a DMA TX on
// every SysTick. To force a flush, we just call the `flush` method,
// which sets the `need_flush` flag, and a DMA TX is started at the
// next SysTick.

void USART::dispatch(const Events::Event &e) {
  switch (e.tag) {
  case Events::EVENT_LOOP_STARTED:
    init();
    mgr->post(Events::USART_INIT, _iusart);
    break;

  case Events::SYSTICK:
    // Skip conditions that don't require us to start a DMA TX.
    if (_need_flush && _tx_size != 0 && !_tx_sending)
      start_tx_dma();
    break;

  default:
    break;
  }
}


// Start a DMA transmission.

void USART::start_tx_dma(void) {
  // Set DMA request to send _tx_size bytes starting at _tx_buff.
  // (Following procedure in Section 8.3.18 of reference manual.)

  // 1. If the stream is enabled, disable it by resetting the EN bit
  //    in the DMA_SxCR register, then read this bit in order to
  //    confirm that there is no ongoing stream operation.
  CLEAR_BIT(_dma->CR, DMA_SxCR_EN);
  while (READ_BIT(_dma->CR, DMA_SxCR_EN)) { __asm("nop"); }

  // 3. Set the memory address in the DMA_SxMA0R register.
  _dma->M0AR = (uintptr_t)_tx_buff;

  // 4. Configure the total number of data items to be transferred in
  //    the DMA_SxNDTR register.
  _dma->NDTR = _tx_size;

  // Clear all interrupt pending flags.
  SET_BIT(DMA1->LIFCR, DMA_LIFCR_CTCIF3);
  SET_BIT(DMA1->LIFCR, DMA_LIFCR_CHTIF3);
  SET_BIT(DMA1->LIFCR, DMA_LIFCR_CTEIF3);
  SET_BIT(DMA1->LIFCR, DMA_LIFCR_CDMEIF3);

  // Ensure all relevant interrupts are enabled.
  SET_BIT(_dma->CR, DMA_SxCR_TCIE);

  // 10. Activate the stream by setting the EN bit in the DMA_SxCR
  //     register.
  SET_BIT(_dma->CR, DMA_SxCR_EN);

  // TODO: SOME OF THIS SHOULD BE DONE *BEFORE* ENABLING THE DMA!
  // Swap DMA buffers for writing and mark that a DMA is in progress.
  _tx_buff_idx = 1 - _tx_buff_idx;
  _tx_buff = _tx_buffs[_tx_buff_idx];
  _tx_size = 0;
  _tx_sending = true;
  _need_flush = false;
}


// USART RX interrupt handler.

void USART::rx_irq(void) {
  // Overrun: clear buffer, return error.
  if (_usart->ISR & USART_ISR_ORE) {
    SET_BIT(_usart->ICR, USART_ICR_ORECF);
    if (mgr) mgr->post(Events::USART_RX_OVERRUN);
    return;
  }

  // Byte received.
  if (_usart->ISR & USART_ISR_RXNE) {
    char c = _usart->RDR;
    if (mgr) mgr->post(Events::USART_RX_CHAR, c);
  }
}


// DMA stream interrupt handler.

void USART::tx_dma_irq(void) {
  // Permit another flush.
  _tx_sending = false;

  // Errors.
  if (DMA1->LISR & DMA_LISR_TEIF3) {
    _tx_error = true;
    SET_BIT(DMA1->LIFCR, DMA_LIFCR_CTEIF3);
  }
  if (DMA1->LISR & DMA_LISR_DMEIF3) {
    _tx_error = true;
    SET_BIT(DMA1->LIFCR, DMA_LIFCR_CDMEIF3);
  }

  // Transfer complete.
  if (DMA1->LISR & DMA_LISR_TCIF3) {
    SET_BIT(DMA1->LIFCR, DMA_LIFCR_CTCIF3);
  }

  if (_tx_error) {
    if (mgr) mgr->post(Events::USART_TX_ERROR);
  }
}


// The addresses for these aren't assigned systematically, hence the
// need for a switch statement here.

USART_TypeDef *USART::usart_base(int iusart) {
  switch (iusart) {
  case 1: return USART1;
  case 2: return USART2;
  case 3: return USART3;
  case 4: return UART4;
  case 5: return UART5;
  case 6: return USART6;
  default: return nullptr;
  }
}


// TODO: DO THIS BETTER -- THERE ARE MULTIPLE POSSIBLE DMA STREAMS FOR
// MOST PERIPHERALS. FOR EXAMPLE, USART3_TX IS ON DMA1_Stream3,
// CHANNEL 4 AND ON DMA1_Stream4, CHANNEL 7.

DMA_Stream_TypeDef *USART::usart_dma_stream(int iusart) {
  switch (iusart) {
  case 1: return DMA2_Stream7;
  case 2: return DMA1_Stream6;
  case 3: return DMA1_Stream3;
  case 4: return DMA1_Stream4;
  case 5: return DMA1_Stream7;
  case 6: return DMA2_Stream6;
  default: return nullptr;
  }
}

static IRQn_Type usart_irqn(int iusart) {
  switch (iusart) {
  case 1: return USART1_IRQn;
  case 2: return USART2_IRQn;
  case 3: return USART3_IRQn;
  case 4: return UART4_IRQn;
  case 5: return UART5_IRQn;
  case 6: return USART6_IRQn;
  default: return NonMaskableInt_IRQn;
  }
}

//----------------------------------------------------------------------
//
//  TESTS
//

#ifdef TEST

#include "doctest.h"
#include "doctest/trompeloeil.hpp"
#include "events_mock.hpp"

TEST_CASE("USART") {
  using trompeloeil::_;

  MockEventWaiter waiter;
  Events::Manager ev(MockEventWaiter::wait_for_event);
  USART usart(3, PD8, GPIO_AF_7, PD9, GPIO_AF_7, DMAChannel { 1, 3, 4 });
  ev += usart;
  MockEventConsumer consumer;
  ev += consumer;

  SUBCASE("RX ISR enqueues correct event") {
    ALLOW_CALL(consumer, dispatch(_));
    ev.drain();
    USART3->RDR = 'x';
    SET_BIT(USART3->ISR, USART_ISR_RXNE);
    usart.rx_irq();
    REQUIRE_CALL(consumer, dispatch(_))
      .WITH(_1.tag == Events::USART_RX_CHAR && _1.param == 'x');
    ev.drain();
    CHECK(ev.pending_count() == 0);
  }

  SUBCASE("RX overrun enqueues correct event") {
    ALLOW_CALL(consumer, dispatch(_));
    ev.drain();
    SET_BIT(USART3->ISR, USART_ISR_ORE);
    usart.rx_irq();
    REQUIRE_CALL(consumer, dispatch(_))
      .WITH(_1.tag == Events::USART_RX_OVERRUN);
    ev.drain();
    CHECK(ev.pending_count() == 0);
  }

  SUBCASE("SysTick triggers TX DMA") {
    ALLOW_CALL(consumer, dispatch(_));
    ev.drain();
    memset(DMA1_Stream3, 0, sizeof(DMA_Stream_TypeDef));
    for (auto c : "abcdef") {
      usart.tx(c);
    }
    usart.flush();
    ev.post(Events::SYSTICK);
    ev.drain();
    CHECK(READ_BIT(DMA1_Stream3->CR, DMA_SxCR_EN) != 0);
  }
}

#endif
