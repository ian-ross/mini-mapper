#include "bsp-nucleo.h"

static void enable_caches(void);
static void configure_clock(void);

enum GPIOSpeed {
  GPIO_SPEED_LOW = 0,
  GPIO_SPEED_MEDIUM = 1,
  GPIO_SPEED_HIGH = 2,
  GPIO_SPEED_VERY_HIGH = 3
};

enum GPIOOutputType {
  GPIO_TYPE_PUSH_PULL = 0,
  GPIO_TYPE_OPEN_DRAIN = 1
};

enum GPIOPUPD {
  GPIO_PUPD_NONE = 0,
  GPIO_PUPD_PULL_UP = 1,
  GPIO_PUPD_PULL_DOWN = 2
};

class Pin {
  private:
  GPIO_TypeDef *port;
  uint16_t pin;

  public:
  Pin(GPIO_TypeDef *port, uint16_t pin) : port(port), pin(pin) {
    // Enable GPIO clock: AHB1ENR bits are one per port, starting from
    // zero, and the port addresses are every 0x0400 starting at the
    // base address.
    uint32_t mask = 1 << (((uint32_t)port - AHB1PERIPH_BASE) / 0x0400UL);
    RCC->AHB1ENR |= mask;
  }

  const GPIO_TypeDef *Port() { return port; }
  const uint16_t PinMask() { return 1 << pin; }

  void Output(GPIOSpeed speed, GPIOOutputType type, GPIOPUPD pupd);

  void Set() { port->ODR |= PinMask(); }
  void Reset() { port->ODR &= ~PinMask(); }
  void Toggle() { port->ODR ^= PinMask(); }
};

void Pin::Output(GPIOSpeed speed, GPIOOutputType type, GPIOPUPD pupd) {
  port->OSPEEDR &= ~(0x03 << (2 * pin));
  port->OSPEEDR |= (int)speed << (2 * pin);
  port->OTYPER &= ~(0x01 << pin);
  port->OTYPER |= (int)type << pin;
  port->PUPDR &= ~(0x03 << (2 * pin));
  port->PUPDR |= (int)pupd << (2 * pin);
  port->MODER &= ~(0x03 << (2 * pin));
  port->MODER |= 0x01 << (2 * pin);
}

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

  // Set up main PLL: 8 MHz (HSE) / 8 (M) * 423 (N) / 2 (P) = 216 MHz.
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
