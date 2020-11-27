#include "bsp-generic.h"

uint32_t AHB1_frequency = 0;
uint32_t APB1_frequency = 0;
uint32_t APB2_frequency = 0;


uint8_t ahb1periph[0x10000];
uint8_t *AHB1PERIPH_BASE = ahb1periph;

uint8_t ahb2periph[0x10000];
uint8_t *AHB2PERIPH_BASE = ahb2periph;

uint8_t apb1periph[0x10000];
uint8_t *APB1PERIPH_BASE = apb1periph;

uint8_t apb2periph[0x10000];
uint8_t *APB2PERIPH_BASE = apb2periph;


void init_peripheral_clocks(void)
{
  RCC->AHB1ENR = 0x00100000;
  RCC->AHB2ENR = 0x00000000;
  RCC->AHB3ENR = 0x00000000;
  RCC->APB1ENR = 0x00000400;
  RCC->APB2ENR = 0x00000000;
}

static void init_mock_gpio(GPIO_TypeDef *gpio)
{
  uint32_t moder = 0x00;
  uint32_t speedr = 0x00;
  uint32_t pupdr = 0x00;
  if (gpio == GPIOA) {
    moder = 0xA8000000;
    speedr = 0x0C000000;
    pupdr = 0x64000000;
  } else if (gpio == GPIOB) {
    moder = 0x00000280;
    speedr = 0x000000C0;
    pupdr = 0x00000100;
  }
  gpio->MODER = moder;
  gpio->OTYPER = 0x00;
  gpio->OSPEEDR = speedr;
  gpio->PUPDR = pupdr;
  gpio->IDR = 0x00;
  gpio->ODR = 0x00;
  gpio->BSRR = 0x00;
  gpio->LCKR = 0x00;
  gpio->AFR[0] = 0x00;
  gpio->AFR[1] = 0x00;
}

void init_mock_gpios(void)
{
  init_mock_gpio(GPIOA);
  init_mock_gpio(GPIOB);
  init_mock_gpio(GPIOC);
  init_mock_gpio(GPIOD);
  init_mock_gpio(GPIOE);
  init_mock_gpio(GPIOF);
  init_mock_gpio(GPIOG);
  init_mock_gpio(GPIOH);
  init_mock_gpio(GPIOI);
  init_mock_gpio(GPIOJ);
  init_mock_gpio(GPIOK);
}

void init_mock_timer(TIM_TypeDef *tim) {
  uint32_t arr = 0x00000000;
  uint32_t af1 = 0x00000000;
  uint32_t af2 = 0x00000000;
  if (tim == TIM2 || tim == TIM5) {
    arr = 0xFFFFFFFF;
  } else if (tim == TIM1 || tim == TIM3 || tim == TIM4 ||
             tim == TIM6 || tim == TIM7 || tim == TIM8) {
    arr = 0x0000FFFF;
  }
  if (tim == TIM1 || tim == TIM8) {
    af1 = 0x00000001;
    af2 = 0x00000001;
  }
  tim->CR1 = 0x00;
  tim->CR2 = 0x00;
  tim->SMCR = 0x00;
  tim->DIER = 0x00;
  tim->SR = 0x00;
  tim->EGR = 0x00;
  tim->CCMR1 = 0x00;
  tim->CCMR2 = 0x00;
  tim->CCER = 0x00;
  tim->CNT = 0x00;
  tim->PSC = 0x00;
  tim->ARR = arr;
  tim->RCR = 0x00;
  tim->CCR1 = 0x00;
  tim->CCR2 = 0x00;
  tim->CCR3 = 0x00;
  tim->CCR4 = 0x00;
  tim->BDTR = 0x00;
  tim->DCR = 0x00;
  tim->DMAR = 0x00;
  tim->OR = 0x00;
  tim->CCMR3 = 0x00;
  tim->CCR5 = 0x00;
  tim->CCR6 = 0x00;
  tim->AF1 = af1;
  tim->AF2 = af2;
}

void init_mock_timers(void) {
  init_mock_timer(TIM1);
  init_mock_timer(TIM2);
  init_mock_timer(TIM3);
  init_mock_timer(TIM4);
  init_mock_timer(TIM5);
  init_mock_timer(TIM6);
  init_mock_timer(TIM7);
  init_mock_timer(TIM8);
  init_mock_timer(TIM9);
  init_mock_timer(TIM10);
  init_mock_timer(TIM11);
  init_mock_timer(TIM12);
  init_mock_timer(TIM13);
  init_mock_timer(TIM14);
}

void init_mock_mcu(void) {
  SET_BIT(RCC->CR, RCC_CR_HSEBYP);
  SET_BIT(RCC->CR, RCC_CR_HSEON);
  SET_BIT(RCC->CR, RCC_CR_HSERDY);
  MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLSRC_HSE, RCC_PLLCFGR_PLLSRC_HSE);
  MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLM, 8 << RCC_PLLCFGR_PLLM_Pos);
  MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLN, 432 << RCC_PLLCFGR_PLLN_Pos);
  MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLP, (2/2-1) << RCC_PLLCFGR_PLLP_Pos);
  MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLQ, 9 << RCC_PLLCFGR_PLLQ_Pos);
  MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLR, 7 << RCC_PLLCFGR_PLLR_Pos);
  SET_BIT(RCC->CR, RCC_CR_PLLON);
  SET_BIT(RCC->CR, RCC_CR_PLLRDY);
  SET_BIT(RCC->APB1ENR, RCC_APB1ENR_PWREN);
  SET_BIT(PWR->CR1, PWR_CR1_ODEN);
  SET_BIT(PWR->CR1, PWR_CR1_ODSWEN);
  MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, 7 << FLASH_ACR_LATENCY_Pos);
  MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_CFGR_PPRE1_DIV16);
  MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, RCC_CFGR_PPRE2_DIV16);
  MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_CFGR_HPRE_DIV1);
  MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_PLL);
  MODIFY_REG(RCC->CFGR, RCC_CFGR_SWS, RCC_CFGR_SWS_PLL);
  MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_CFGR_PPRE1_DIV8);
  MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, RCC_CFGR_PPRE2_DIV4);
  const uint32_t core_clock = 216000000;
  AHB1_frequency = core_clock / 1;
  APB1_frequency = core_clock / 8 * 2;
  APB2_frequency = core_clock / 4 * 2;
  SystemCoreClockUpdate();

  init_peripheral_clocks();
  init_mock_gpios();
  init_mock_timers();
}
