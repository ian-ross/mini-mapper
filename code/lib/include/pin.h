#ifndef _H_PIN_
#define _H_PIN_

#include <stdint.h>

#include "bsp.h"


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
  void Input(GPIOSpeed speed);
  void Alternate(uint8_t af);

  void Set() { port->ODR |= PinMask(); }
  void Reset() { port->ODR &= ~PinMask(); }
  void Toggle() { port->ODR ^= PinMask(); }
};

#endif
