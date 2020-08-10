#include "pin.h"

void Pin::Output(GPIOSpeed speed, GPIOOutputType type, GPIOPUPD pupd) {
  MODIFY_REG(port->OSPEEDR, 0x03 << (2 * pin), (int)speed << (2 * pin));
  MODIFY_REG(port->OTYPER, 0x01 << pin, (int)type << pin);
  MODIFY_REG(port->PUPDR, 0x03 << (2 * pin), (int)pupd << (2 * pin));
  MODIFY_REG(port->MODER, 0x03 << (2 * pin), 0x01 << (2 * pin));
}

void Pin::Input(GPIOSpeed speed) {
  MODIFY_REG(port->OSPEEDR, 0x03 << (2 * pin), (int)speed << (2 * pin));
  MODIFY_REG(port->MODER, 0x03 << (2 * pin), 0x00 << (2 * pin));
}

void Pin::Alternate(uint8_t af) {
  MODIFY_REG(port->MODER, 0x03 << (2 * pin), 0x02 << (2 * pin));
  MODIFY_REG(port->AFR[pin / 8], 0x0F << 4 * (pin % 8), af << 4 * (pin % 8));
  int wait = 0;
  while (wait < 10) { ++wait; }
}
