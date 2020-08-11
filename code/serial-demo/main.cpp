// TODO:
//
// - Transmit buffering + DMA
// - Line-based receive callback
// - Error handling
// - Good C++ API (singleton)

#include <stdint.h>

#include "bsp/nucleo.h"
#include "pin.h"
#include "stm32f767xx.h"
#include "stm32f7xx.h"

static void enable_caches(void);
static void configure_clock(void);

static void serial_init();
static void serial_rx(void);
static void serial_tx(char c);

static void serial_print(const char *str);
//static void serial_print(int i);
template <typename T> static void serial_println(T val);
static volatile bool serial_flush_needed = false;
static void serial_flush(void);
static void serial_do_flush(void);

// SysTick is set to run at 1 kHz.

volatile uint32_t systick_count = 0;

extern "C" void SysTick_Handler(void) {
  ++systick_count;
}

void delay_ms(uint32_t ms)
{
  systick_count = 0;
  while (systick_count < ms) {
    __WFI();
    if (serial_flush_needed) {
      serial_flush_needed = false;
      serial_do_flush();
    }
  }
}

int main(void)
{
  // Enable L1 instruction and data caches.
  enable_caches();

  // Initialise clock to 216 MHz using Nucleo 8 MHz HSE bypass input.
  configure_clock();

  // Set SysTick frequency to 1 kHz.
  SysTick_Config(SystemCoreClock / 1000);

  // Initialise GPIO: LED1 pin as output.
  Pin LED1(LED1_PORT, LED1_PIN);
  LED1.Output(GPIO_SPEED_VERY_HIGH, GPIO_TYPE_PUSH_PULL, GPIO_PUPD_NONE);

  // Initialise serial port communications with ST-Link.
  serial_init();

  serial_print("> ");
  serial_do_flush();
  while (1) {
    LED1.Toggle();
    delay_ms(100);
  }
}

static void enable_caches(void) {
  SCB_EnableICache();
  SCB_EnableDCache();
}

static void configure_clock(void) {
  // Enable HSE with bypass input (8 MHz input clock from Nucleo).
  SET_BIT(RCC->CR, RCC_CR_HSEBYP);
  SET_BIT(RCC->CR, RCC_CR_HSEON);
  while (!READ_BIT(RCC->CR, RCC_CR_HSERDY)) { __asm("nop"); }

  // Disable PLL (can't do this if the system clock is already being
  // driven by the PLL, so don't come back in here afterwards!).
  CLEAR_BIT(RCC->CR, RCC_CR_PLLON);
  while (READ_BIT(RCC->CR, RCC_CR_PLLRDY)) { __asm("nop"); }

  // Set up main PLL: 8 MHz (HSE) / 8 (M) * 432 (N) / 2 (P) = 216 MHz.
  MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLSRC_HSE, RCC_PLLCFGR_PLLSRC_HSE);
  MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLM, 8 << RCC_PLLCFGR_PLLM_Pos);
  MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLN, 432 << RCC_PLLCFGR_PLLN_Pos);
  MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLP, (2/2-1) << RCC_PLLCFGR_PLLP_Pos);

  // Set default values for other PLLs (Q = 9 gives 48 MHz for the USB
  // clock, which is required for that to work).
  MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLQ, 9 << RCC_PLLCFGR_PLLQ_Pos);
  MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLR, 7 << RCC_PLLCFGR_PLLR_Pos);

  // Enable PLL.
  SET_BIT(RCC->CR, RCC_CR_PLLON);
  while (!READ_BIT(RCC->CR, RCC_CR_PLLRDY)) { __asm("nop"); }

  // Activate power over-drive to reach 216 MHz.
  SET_BIT(RCC->APB1ENR, RCC_APB1ENR_PWREN);
  // Delay for clock enable...
  (void)READ_BIT(RCC->APB1ENR, RCC_APB1ENR_PWREN);
  SET_BIT(PWR->CR1, PWR_CR1_ODEN);
  while (!READ_BIT(PWR->CSR1, PWR_CSR1_ODRDY)) { __asm("nop"); }
  SET_BIT(PWR->CR1, PWR_CR1_ODSWEN);
  while (!READ_BIT(PWR->CSR1, PWR_CSR1_ODSWRDY)) { __asm("nop"); }

  // Running the system clock at 216 MHz means we need 7 wait states
  // for the flash (see Table 7 in the STM32F76xxx reference manual).
  MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, 7 << FLASH_ACR_LATENCY_Pos);

  // Select PLL as system clock source and configure HCLK, PCLK1 and
  // PCLK2 clock dividers. Set APB dividers to safe values, then
  // update system clock divider and source before setting APB
  // dividers to final values.
  MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_CFGR_PPRE1_DIV16);
  MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, RCC_CFGR_PPRE2_DIV16);
  MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_CFGR_HPRE_DIV1);
  MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_PLL);
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) { __asm("nop"); }
  MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_CFGR_PPRE1_DIV4);
  MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, RCC_CFGR_PPRE2_DIV2);

  // Update CMSIS system core clock.
  SystemCoreClockUpdate();
}

template <typename T> static void serial_println(T val) {
  serial_print(val);
  serial_tx('\r');
  serial_tx('\n');
}

static void serial_print(const char *str) {
  while (*str) {
    serial_tx(*str);
    ++str;
  }
}

// const int MAX_DIGITS = 16;

// static void serial_print(int i) {
//   char buff[MAX_DIGITS + 1];
//   buff[MAX_DIGITS] = '\0';
//   int idx = MAX_DIGITS;
//   do {
//     buff[--idx] = '0' + i % 10;
//     i /= 10;
//   } while (i > 0);
//   while (buff[idx])
//     serial_out(buff[idx++]);
// }

const int RX_BUFSIZE = 80;
static char rx_buff[RX_BUFSIZE];
static int rx_pos = 0;

const int TX_BUFSIZE = 128;
static char tx_buff1[TX_BUFSIZE], tx_buff2[TX_BUFSIZE];
static char *tx_buffs[] = { tx_buff1, tx_buff2 };
static int tx_buff_idx = 0;
static char *tx_buff = tx_buffs[0];
static int tx_size = 0;
static volatile bool tx_sending = false;
static bool tx_error = false;

// USART3 is connected to the ST-Link on the STM32F767ZI Nucleo board.
// USART3 is on DMA1, stream 3, channel 4.
DMA_Stream_TypeDef *serial_dma = DMA1_Stream3;

extern "C" void USART3_IRQHandler(void) {
  serial_rx();
}

extern "C" void DMA1_Stream3_IRQHandler(void) {
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

static void serial_flush(void) {
  serial_flush_needed = true;
}

static void serial_do_flush(void) {
  DMA_TypeDef *dma1 = DMA1;
  (void)dma1;

  if (tx_size == 0) return;
  while (tx_sending) { __WFI(); }

  tx_sending = true;

  // Set DMA request to send tx_send_count bytes starting at tx_buff +
  // tx_read_pos. (Following procedure in Section 8.3.18 of reference
  // manual.)

  // 1. If the stream is enabled, disable it by resetting the EN bit
  //    in the DMA_SxCR register, then read this bit in order to
  //    confirm that there is no ongoing stream operation.
  CLEAR_BIT(serial_dma->CR, DMA_SxCR_EN);
  while (READ_BIT(serial_dma->CR, DMA_SxCR_EN)) { __asm("nop"); }

  // 3. Set the memory address in the DMA_SxMA0R register.
  serial_dma->M0AR = (uint32_t)tx_buff;

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

static void serial_tx(char c) {
  // If we would overflow the TX buffer here, flush the current buffer
  // to DMA and switch to the other.
  if (tx_size >= TX_BUFSIZE) serial_flush();

  // Buffer character.
  tx_buff[tx_size++] = c;
}

static void serial_error(const char *msg) {
  serial_println("");
  serial_print("!");
  serial_println(msg);
  serial_print("> ");
  serial_flush();
}

static void serial_rx(void) {
  // Overrun: clear buffer, return error.
  if (USART3->ISR & USART_ISR_ORE) {
    SET_BIT(USART3->ICR, USART_ICR_ORECF);
    rx_pos = 0;
    serial_error("ORE");
    return;
  }

  // Byte received.
  if (USART3->ISR & USART_ISR_RXNE) {
    char c = USART3->RDR;

    // Buffer overflow: clear buffer, return error.
    if (rx_pos >= RX_BUFSIZE) {
      rx_pos = 0;
      serial_error("RXOFLOW");
      return;
    }

    switch (c) {
    case '\r':
      // Enter handling.
      rx_buff[rx_pos] = '\0';
      rx_pos = 0;
      serial_println("");
      serial_print("RX: ");
      serial_println(rx_buff);
      serial_print("> ");
      serial_flush();
      break;

    case '\b':
      // Backspace handling. This works with Minicom, don't know if it
      // will work with other terminal programs.
      if (rx_pos > 0) {
        --rx_pos;
        serial_print("\b \b");
        serial_flush();
      }
      break;

    default:
      // Normal case: add character to buffer and echo.
      rx_buff[rx_pos++] = c;
      serial_tx(c);
      serial_flush();
    }
  }
}

static void serial_init() {
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
  Pin TX(GPIOD, 8);
  Pin RX(GPIOD, 9);

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
  serial_dma->PAR = (uint32_t)&USART3->TDR;

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
