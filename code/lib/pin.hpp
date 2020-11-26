#ifndef _H_PIN_
#define _H_PIN_

#include <stdint.h>

#include "bsp-generic.h"


// GPIO pin speed settings.

enum GPIOSpeed {
  GPIO_SPEED_LOW = 0,
  GPIO_SPEED_MEDIUM = 1,
  GPIO_SPEED_HIGH = 2,
  GPIO_SPEED_VERY_HIGH = 3
};


// GPIO pin output types.

enum GPIOOutputType {
  GPIO_TYPE_PUSH_PULL = 0,
  GPIO_TYPE_OPEN_DRAIN = 1
};


// GPIO pull-up/-down resistor settings.

enum GPIOPUPD {
  GPIO_PUPD_NONE = 0,
  GPIO_PUPD_PULL_UP = 1,
  GPIO_PUPD_PULL_DOWN = 2
};


// GPIO alternate functions.

enum GPIOAF {
  GPIO_AF_1 = 1,
  GPIO_AF_2,
  GPIO_AF_3,
  GPIO_AF_4,
  GPIO_AF_5,
  GPIO_AF_6,
  GPIO_AF_7,
  GPIO_AF_8,
  GPIO_AF_9,
  GPIO_AF_10,
  GPIO_AF_11,
  GPIO_AF_12,
  GPIO_AF_13,
  GPIO_AF_14,
  GPIO_AF_15,
};


// Abstraction of a GPIO pin. This just carries the I/O port and pin
// number around together and provides convenience methods for setting
// up pin configurations. All methods are defined as `const` because
// they don't modify the `Pin` values themselves (and it allows us to
// have a `const Pin` defined for each pin on the STM32F7xx).

class Pin {
public:

  Pin(GPIO_TypeDef *port, uint16_t pin) : _port(port), _pin(pin) { }
  Pin(const Pin &other) = default;

  bool operator==(const Pin &other) const {
    return _port == other._port && _pin == other._pin;
  }

  const GPIO_TypeDef *port() const { return _port; }
  const uint16_t pin_mask() const { return 1 << _pin; }

  // Set pin as GPIO output.
  void output(GPIOSpeed speed, GPIOOutputType type, GPIOPUPD pupd) const;

  // Set pin as GPIO input.
  void input(GPIOSpeed speed) const;

  // Set pin as analog input.
  void analog(void) const;

  // Set pin to alternate function.
  void alternate(GPIOAF af) const;

  // Outputs: set, reset, toggle.
  void set() const { _port->ODR |= pin_mask(); }
  void reset() const { _port->ODR &= ~pin_mask(); }
  void toggle() const { _port->ODR ^= pin_mask(); }

  // Inputs: read.
  // TBD

private:

  GPIO_TypeDef *_port;
  uint16_t _pin;
};


// The LQFP-144 STM32F767xx has ports A-G, each with 16 pins and port
// H with 2 pins. Define all these as `const` so that the compiler can
// elide them if they're not used.

const Pin PA0(GPIOA, 0);
const Pin PA1(GPIOA, 1);
const Pin PA2(GPIOA, 2);
const Pin PA3(GPIOA, 3);
const Pin PA4(GPIOA, 4);
const Pin PA5(GPIOA, 5);
const Pin PA6(GPIOA, 6);
const Pin PA7(GPIOA, 7);
const Pin PA8(GPIOA, 8);
const Pin PA9(GPIOA, 9);
const Pin PA10(GPIOA, 10);
const Pin PA11(GPIOA, 11);
const Pin PA12(GPIOA, 12);
const Pin PA13(GPIOA, 13);
const Pin PA14(GPIOA, 14);
const Pin PA15(GPIOA, 15);

const Pin PB0(GPIOB, 0);
const Pin PB1(GPIOB, 1);
const Pin PB2(GPIOB, 2);
const Pin PB3(GPIOB, 3);
const Pin PB4(GPIOB, 4);
const Pin PB5(GPIOB, 5);
const Pin PB6(GPIOB, 6);
const Pin PB7(GPIOB, 7);
const Pin PB8(GPIOB, 8);
const Pin PB9(GPIOB, 9);
const Pin PB10(GPIOB, 10);
const Pin PB11(GPIOB, 11);
const Pin PB12(GPIOB, 12);
const Pin PB13(GPIOB, 13);
const Pin PB14(GPIOB, 14);
const Pin PB15(GPIOB, 15);

const Pin PC0(GPIOC, 0);
const Pin PC1(GPIOC, 1);
const Pin PC2(GPIOC, 2);
const Pin PC3(GPIOC, 3);
const Pin PC4(GPIOC, 4);
const Pin PC5(GPIOC, 5);
const Pin PC6(GPIOC, 6);
const Pin PC7(GPIOC, 7);
const Pin PC8(GPIOC, 8);
const Pin PC9(GPIOC, 9);
const Pin PC10(GPIOC, 10);
const Pin PC11(GPIOC, 11);
const Pin PC12(GPIOC, 12);
const Pin PC13(GPIOC, 13);
const Pin PC14(GPIOC, 14);
const Pin PC15(GPIOC, 15);

const Pin PD0(GPIOD, 0);
const Pin PD1(GPIOD, 1);
const Pin PD2(GPIOD, 2);
const Pin PD3(GPIOD, 3);
const Pin PD4(GPIOD, 4);
const Pin PD5(GPIOD, 5);
const Pin PD6(GPIOD, 6);
const Pin PD7(GPIOD, 7);
const Pin PD8(GPIOD, 8);
const Pin PD9(GPIOD, 9);
const Pin PD10(GPIOD, 10);
const Pin PD11(GPIOD, 11);
const Pin PD12(GPIOD, 12);
const Pin PD13(GPIOD, 13);
const Pin PD14(GPIOD, 14);
const Pin PD15(GPIOD, 15);

const Pin PE0(GPIOE, 0);
const Pin PE1(GPIOE, 1);
const Pin PE2(GPIOE, 2);
const Pin PE3(GPIOE, 3);
const Pin PE4(GPIOE, 4);
const Pin PE5(GPIOE, 5);
const Pin PE6(GPIOE, 6);
const Pin PE7(GPIOE, 7);
const Pin PE8(GPIOE, 8);
const Pin PE9(GPIOE, 9);
const Pin PE10(GPIOE, 10);
const Pin PE11(GPIOE, 11);
const Pin PE12(GPIOE, 12);
const Pin PE13(GPIOE, 13);
const Pin PE14(GPIOE, 14);
const Pin PE15(GPIOE, 15);

const Pin PF0(GPIOF, 0);
const Pin PF1(GPIOF, 1);
const Pin PF2(GPIOF, 2);
const Pin PF3(GPIOF, 3);
const Pin PF4(GPIOF, 4);
const Pin PF5(GPIOF, 5);
const Pin PF6(GPIOF, 6);
const Pin PF7(GPIOF, 7);
const Pin PF8(GPIOF, 8);
const Pin PF9(GPIOF, 9);
const Pin PF10(GPIOF, 10);
const Pin PF11(GPIOF, 11);
const Pin PF12(GPIOF, 12);
const Pin PF13(GPIOF, 13);
const Pin PF14(GPIOF, 14);
const Pin PF15(GPIOF, 15);

const Pin PG0(GPIOG, 0);
const Pin PG1(GPIOG, 1);
const Pin PG2(GPIOG, 2);
const Pin PG3(GPIOG, 3);
const Pin PG4(GPIOG, 4);
const Pin PG5(GPIOG, 5);
const Pin PG6(GPIOG, 6);
const Pin PG7(GPIOG, 7);
const Pin PG8(GPIOG, 8);
const Pin PG9(GPIOG, 9);
const Pin PG10(GPIOG, 10);
const Pin PG11(GPIOG, 11);
const Pin PG12(GPIOG, 12);
const Pin PG13(GPIOG, 13);
const Pin PG14(GPIOG, 14);
const Pin PG15(GPIOG, 15);

const Pin PH0(GPIOH, 0);
const Pin PH1(GPIOH, 1);

#endif
