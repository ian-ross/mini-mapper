#include "bsp/generic.h"

#include "pin.hpp"
#include "term.hpp"

Term &term = Term::instance();

// USART3 is connected to the ST-Link on the STM32F767ZI Nucleo board.
// USART3 is on DMA1, stream 3, channel 4.
DMA_Stream_TypeDef *serial_dma = DMA1_Stream3;

Term::Term() {
  // Disable USART.
  CLEAR_BIT(USART3->CR1, USART_CR1_UE);

  // Enable USART3 clock.
  SET_BIT(RCC->APB1ENR, RCC_APB1ENR_USART3EN);
  (void)READ_BIT(RCC->APB1ENR, RCC_APB1ENR_USART3EN);

  // Select system clock (216 MHz) as clock source for USART3.
  MODIFY_REG(RCC->DCKCFGR2, RCC_DCKCFGR2_USART3SEL, 0x01 << RCC_DCKCFGR2_USART3SEL_Pos);

  // Set word length (M1 = 0, M0 = 0 => 1 start bit, 8 data bits, n
  // stop bits).
  CLEAR_BIT(USART3->CR1, USART_CR1_M0);
  CLEAR_BIT(USART3->CR1, USART_CR1_M1);

  // Disable parity.
  CLEAR_BIT(USART3->CR1, USART_CR1_PCE);

  // Disable auto baudrate detection.
  CLEAR_BIT(USART3->CR2, USART_CR2_ABREN);

  // Send/receive LSB first.
  CLEAR_BIT(USART3->CR2, USART_CR2_MSBFIRST);

  // Set oversampling rate to 16 and select baud rate 115200 based on
  // USART clock running at 216 MHz (value taken from Table 220 in
  // STM32F767ZI reference manual). (216000000 / 115200 = 0x0753)
  CLEAR_BIT(USART3->CR1, USART_CR1_OVER8);
  USART3->BRR = 0x0753;

  // One stop bit (USART.CR2.STOP[1:0] = 0 => 1 stop bit).
  MODIFY_REG(USART3->CR2, USART_CR2_STOP_Msk, 0);

  // Enable USART, receiver and transmitter.
  SET_BIT(USART3->CR1, USART_CR1_UE);
  SET_BIT(USART3->CR1, USART_CR1_RE);
  SET_BIT(USART3->CR1, USART_CR1_TE);

  // Set up USART3 on pins PD8 (TX) and PD9 (RX). USART3 is AF7 on
  // these pins.
  const Pin &TX = PD8;
  const Pin &RX = PD9;

  // Pin configuration: set PD8 to output (PP, pull-up), PD9 to input,
  // set both to AF7.
  TX.Output(GPIO_SPEED_VERY_HIGH, GPIO_TYPE_PUSH_PULL, GPIO_PUPD_PULL_UP);
  RX.Input(GPIO_SPEED_VERY_HIGH);
  TX.Alternate(7);
  RX.Alternate(7);

  // Receive interrupt setup: enable USART3 interrupts in NVIC and
  // enable RX interrupt (also handles overrun errors).
  NVIC_EnableIRQ(USART3_IRQn);
  SET_BIT(USART3->CR1, USART_CR1_RXNEIE);

  // Transmit DMA setup. (Following procedure in Section 8.3.18 of
  // reference manual.)

  // Enable DMA transmitter for UART.
  SET_BIT(USART3->CR3, USART_CR3_DMAT);

  // Enable clock for DMA1.
  SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_DMA1EN);

  // 2. Set the peripheral port register address in the DMA_SxPAR
  //    register.
  serial_dma->PAR = (uintptr_t)&USART3->TDR;

  // 5. Select the DMA channel (request) using CHSEL[3:0] in the
  //    DMA_SxCR register.

  MODIFY_REG(serial_dma->CR, DMA_SxCR_CHSEL_Msk, 0x04 << DMA_SxCR_CHSEL_Pos);

  // DMA is flow controller.
  CLEAR_BIT(serial_dma->CR, DMA_SxCR_PFCTRL);

  // 7. Configure the stream priority using the PL[1:0] bits in the
  //    DMA_SxCR register.

  // Medium priority.
  MODIFY_REG(serial_dma->CR, DMA_SxCR_PL_Msk, 0x02 << DMA_SxCR_PL_Pos);

  // 8. Configure the FIFO usage (enable or disable, threshold in
  //    transmission and reception).
  CLEAR_BIT(serial_dma->FCR, DMA_SxFCR_DMDIS);            // No FIFO: direct mode.

  // 9. Configure the data transfer direction, peripheral and memory
  //    incremented/fixed mode, single or burst transactions,
  //    peripheral and memory data widths, circular mode,
  //    double-buffer mode and interrupts after half and/or full
  //    transfer, and/or errors in the DMA_SxCR register.

  // Memory-to-peripheral.
  MODIFY_REG(serial_dma->CR, DMA_SxCR_DIR_Msk, 0x01 << DMA_SxCR_DIR_Pos);

  // Increment memory, no increment peripheral.
  SET_BIT(serial_dma->CR, DMA_SxCR_MINC);
  CLEAR_BIT(serial_dma->CR, DMA_SxCR_PINC);

  // No burst at either end.
  MODIFY_REG(serial_dma->CR, DMA_SxCR_MBURST_Msk, 0x00 << DMA_SxCR_MBURST_Pos);
  MODIFY_REG(serial_dma->CR, DMA_SxCR_PBURST_Msk, 0x00 << DMA_SxCR_PBURST_Pos);

  // Byte size at both ends.
  MODIFY_REG(serial_dma->CR, DMA_SxCR_MSIZE_Msk, 0x00 << DMA_SxCR_MSIZE_Pos);
  MODIFY_REG(serial_dma->CR, DMA_SxCR_PSIZE_Msk, 0x00 << DMA_SxCR_PSIZE_Pos);

  // No circular mode, no double buffering.
  CLEAR_BIT(serial_dma->CR, DMA_SxCR_CIRC);
  CLEAR_BIT(serial_dma->CR, DMA_SxCR_DBM);

  // Transfer complete interrupt, no half-transfer interrupt, transfer
  // error interrupt and direct mode error interrupt.
  SET_BIT(serial_dma->CR, DMA_SxCR_TCIE);
  CLEAR_BIT(serial_dma->CR, DMA_SxCR_HTIE);
  SET_BIT(serial_dma->CR, DMA_SxCR_TEIE);
  SET_BIT(serial_dma->CR, DMA_SxCR_DMEIE);

  // Transmit DMA interrupt setup: enable DMA interrupts for DMA
  // channel attached to USART3.
  NVIC_EnableIRQ(DMA1_Stream3_IRQn);
}


void Term::rx_irq(void) {
  // Overrun: clear buffer, return error.
  if (USART3->ISR & USART_ISR_ORE) {
    SET_BIT(USART3->ICR, USART_ICR_ORECF);
    rx_pos = 0;
    error("ORE");
    return;
  }

  // Byte received.
  if (USART3->ISR & USART_ISR_RXNE) {
    char c = USART3->RDR;

    // Buffer overflow: clear buffer, return error.
    if (rx_pos >= RX_BUFSIZE) {
      rx_pos = 0;
      error("RXOFLOW");
      return;
    }

    switch (c) {
    case '\r':
      // Enter handling.
      rx_buff[rx_pos] = '\0';
      rx_pos = 0;
      println("");
      print("RX: ");
      println(rx_buff);
      print("> ");
      flush();
      break;

    case '\b':
      // Backspace handling. This works with Minicom, don't know if it
      // will work with other terminal programs.
      if (rx_pos > 0) {
        --rx_pos;
        print("\b \b");
        flush();
      }
      break;

    default:
      // Normal case: add character to buffer and echo.
      rx_buff[rx_pos++] = c;
      tx(c);
      flush();
    }
  }
}

void Term::tx(char c) {
  // If we would overflow the TX buffer here, flush the current buffer
  // to DMA and switch to the other.
  if (tx_size >= TX_BUFSIZE) Term::flush();

  // Buffer character.
  tx_buff[tx_size++] = c;
}

template <typename T> void Term::println(T val) {
  print(val);
  tx('\r');
  tx('\n');
}

void Term::print(const char *str) {
  while (*str) {
    tx(*str);
    ++str;
  }
}

const int MAX_DIGITS = 16;

void Term::print(int i) {
  char buff[MAX_DIGITS + 1];
  buff[MAX_DIGITS] = '\0';
  int idx = MAX_DIGITS;
  do {
    buff[--idx] = '0' + i % 10;
    i /= 10;
  } while (i > 0);
  while (buff[idx])
    tx(buff[idx++]);
}

void Term::error(const char *msg) {
  println("");
  print("!");
  println(msg);
  print("> ");
  flush();
}


void Term::idle_flush(void) {
  if (!need_flush || tx_size == 0) return;
  while (tx_sending) { __WFI(); }

  tx_sending = true;
  need_flush = false;

  // Set DMA request to send tx_send_count bytes starting at tx_buff +
  // tx_read_pos. (Following procedure in Section 8.3.18 of reference
  // manual.)

  // 1. If the stream is enabled, disable it by resetting the EN bit
  //    in the DMA_SxCR register, then read this bit in order to
  //    confirm that there is no ongoing stream operation.

  // USART3 is connected to the ST-Link on the STM32F767ZI Nucleo board.
  // USART3 is on DMA1, stream 3, channel 4.
  DMA_Stream_TypeDef *serial_dma = DMA1_Stream3;

  CLEAR_BIT(serial_dma->CR, DMA_SxCR_EN);
  while (READ_BIT(serial_dma->CR, DMA_SxCR_EN)) { __asm("nop"); }

  // 3. Set the memory address in the DMA_SxMA0R register.
  serial_dma->M0AR = (uintptr_t)tx_buff;

  // 4. Configure the total number of data items to be transferred in
  //    the DMA_SxNDTR register.
  serial_dma->NDTR = tx_size;

  // Clear all interrupt pending flags.
  SET_BIT(DMA1->LIFCR, DMA_LIFCR_CTCIF3);
  SET_BIT(DMA1->LIFCR, DMA_LIFCR_CHTIF3);
  SET_BIT(DMA1->LIFCR, DMA_LIFCR_CTEIF3);
  SET_BIT(DMA1->LIFCR, DMA_LIFCR_CDMEIF3);

  // Ensure all relevant interrupts are enabled.
  SET_BIT(serial_dma->CR, DMA_SxCR_TCIE);

  // 10. Activate the stream by setting the EN bit in the DMA_SxCR register.
  SET_BIT(serial_dma->CR, DMA_SxCR_EN);

  // Swap DMA buffers for writing.
  tx_buff_idx = 1 - tx_buff_idx;
  tx_buff = tx_buffs[tx_buff_idx];
  tx_size = 0;
}

void Term::tx_dma_irq(void) {
  // Permit another flush.
  tx_sending = false;

  // Errors.
  if (DMA1->LISR & DMA_LISR_TEIF3) {
    tx_error = true;
    SET_BIT(DMA1->LIFCR, DMA_LIFCR_CTEIF3);
  }
  if (DMA1->LISR & DMA_LISR_DMEIF3) {
    tx_error = true;
    SET_BIT(DMA1->LIFCR, DMA_LIFCR_CDMEIF3);
  }

  // Transfer complete.
  if (DMA1->LISR & DMA_LISR_TCIF3) {
    SET_BIT(DMA1->LIFCR, DMA_LIFCR_CTCIF3);
  }
}
