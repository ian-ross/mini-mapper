// TODO:
//
// - Transmit buffering + DMA
// - Receive interrupt
// - Receive buffering
// - Line-based receive callback
// - Error handling

#include <stdint.h>

#include "bsp-nucleo.h"

#include "pin.h"

static void enable_caches(void);
static void configure_clock(void);

static void serial_init();
static void serial_out(char c);
static void serial_print(const char *str);
static void serial_print(int i);
template <typename T> static void serial_println(T val);


// SysTick is set to run at 1 kHz.

volatile uint32_t systick_count = 0;

extern "C" void SysTick_Handler(void) {
  ++systick_count;
}

void delay_ms(uint32_t ms)
{
  systick_count = 0;
  while (systick_count < ms) { __asm("nop"); }
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

  int count = 0;
  while (1) {
    if (count++ % 20 == 0) {
      serial_print("COUNT: ");
      serial_println(count / 20);
    }
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

static void serial_out(char c) {
  while (!(USART3->ISR & USART_ISR_TXE)) { __asm("nop"); }
  USART3->TDR = c;
}

template <typename T> static void serial_println(T val) {
  serial_print(val);
  serial_out('\r');
  serial_out('\n');
}

static void serial_print(const char *str) {
  while (*str) {
    serial_out(*str);
    ++str;
  }
}

const int MAX_DIGITS = 16;

static void serial_print(int i) {
  char buff[MAX_DIGITS + 1];
  buff[MAX_DIGITS] = '\0';
  int idx = MAX_DIGITS;
  do {
    buff[--idx] = '0' + i % 10;
    i /= 10;
  } while (i > 0);
  while (buff[idx])
    serial_out(buff[idx++]);
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

  // Pin configuration:
  //  - Set PD8 to output, PD9 to input
  //  - Set PD8 output type (PP, pull-up)
  TX.Output(GPIO_SPEED_VERY_HIGH, GPIO_TYPE_PUSH_PULL, GPIO_PUPD_PULL_UP);
  RX.Input(GPIO_SPEED_VERY_HIGH);
  //  - Set PD8 and PD9 to AF7
  TX.Alternate(7);
  RX.Alternate(7);

  // TODO: INTERRUPTS AND/OR DMA
}
