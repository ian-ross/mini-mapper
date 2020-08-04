#include "bsp-nucleo.h"
#include "stm32f767xx.h"

volatile uint32_t systick_count = 0;

extern "C" void SysTick_Handler(void) {
  ++systick_count;
}

// Delay by counting SysTicks.
void delay()
{
  systick_count = 0;
  while (systick_count < 500) { __asm("nop"); }
}

int main(void)
{
  // Don't do any additional clock initialisation here! Dumb
  // example...

  // Initialise GPIO: LED1 pin as output.

  GPIO_TypeDef *port = GPIOB;
  uint8_t pin = 0;

  // Enable clock for GPIOB: need to do this before the other setup.
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

  // Very high speed.
  port->OSPEEDR &= ~(0x03 << (2 * pin));
  port->OSPEEDR |= 0x03 << (2 * pin);

  // Push-pull output.
  port->OTYPER &= ~(0x01 << pin);

  // No pull-up, no pull-down.
  port->PUPDR &= ~(0x03 << (2 * pin));
  port->PUPDR |= 0x00 << (2 * pin);

  // General purpose output.
  port->MODER &= ~(0x03 << (2 * pin));
  port->MODER |= 0x01 << (2 * pin);

  // SysTick event every 10,000 clocks, whatever that is.
  SysTick_Config(10000);

  while (1) {
    // Toggle LED.
    port->ODR ^= 1 << pin;
    delay();
  }
}
