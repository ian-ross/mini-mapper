#include "setup.hpp"

#include "bsp-nucleo.h"

uint32_t AHB1_frequency = 0;
uint32_t APB1_frequency = 0;
uint32_t APB2_frequency = 0;

// Enable L1 instruction and data caches.
void enable_caches(void) {
  SCB_EnableICache();
  SCB_EnableDCache();
}

// Initialise clock to 216 MHz using Nucleo 8 MHz HSE bypass input.
void configure_clock(void) {
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
  // dividers to final values, to run AHB at 216 MHz, APB1 at 27 MHz
  // (216 / 8) (max. permitted = 45 MHz) and APB2 at 54 MHz (216 / 4)
  // (max. permitted = 90 MHz).
  MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_CFGR_PPRE1_DIV16);
  MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, RCC_CFGR_PPRE2_DIV16);
  MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_CFGR_HPRE_DIV1);
  MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_PLL);
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) { __asm("nop"); }
  MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_CFGR_PPRE1_DIV8);
  MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, RCC_CFGR_PPRE2_DIV4);
  const uint32_t core_clock = 216000000;
  AHB1_frequency = core_clock / 1;
  APB1_frequency = core_clock / 8;
  APB2_frequency = core_clock / 4;

  // Update CMSIS system core clock.
  SystemCoreClockUpdate();
}

void fatal(const char *msg, int *param) {
  while (true) { __asm("nop"); }
}
