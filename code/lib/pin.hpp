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
  bool read(void) const { return _port->IDR & pin_mask(); }


private:

  GPIO_TypeDef *_port;
  uint16_t _pin;
};


// The LQFP-144 STM32F767xx has ports A-G, each with 16 pins and port
// H with 2 pins. Define all these as `const` so that the compiler can
// elide them if they're not used.

inline const Pin PA0(GPIOA, 0);
inline const Pin PA1(GPIOA, 1);
inline const Pin PA2(GPIOA, 2);
inline const Pin PA3(GPIOA, 3);
inline const Pin PA4(GPIOA, 4);
inline const Pin PA5(GPIOA, 5);
inline const Pin PA6(GPIOA, 6);
inline const Pin PA7(GPIOA, 7);
inline const Pin PA8(GPIOA, 8);
inline const Pin PA9(GPIOA, 9);
inline const Pin PA10(GPIOA, 10);
inline const Pin PA11(GPIOA, 11);
inline const Pin PA12(GPIOA, 12);
inline const Pin PA13(GPIOA, 13);
inline const Pin PA14(GPIOA, 14);
inline const Pin PA15(GPIOA, 15);

inline const Pin PB0(GPIOB, 0);
inline const Pin PB1(GPIOB, 1);
inline const Pin PB2(GPIOB, 2);
inline const Pin PB3(GPIOB, 3);
inline const Pin PB4(GPIOB, 4);
inline const Pin PB5(GPIOB, 5);
inline const Pin PB6(GPIOB, 6);
inline const Pin PB7(GPIOB, 7);
inline const Pin PB8(GPIOB, 8);
inline const Pin PB9(GPIOB, 9);
inline const Pin PB10(GPIOB, 10);
inline const Pin PB11(GPIOB, 11);
inline const Pin PB12(GPIOB, 12);
inline const Pin PB13(GPIOB, 13);
inline const Pin PB14(GPIOB, 14);
inline const Pin PB15(GPIOB, 15);

inline const Pin PC0(GPIOC, 0);
inline const Pin PC1(GPIOC, 1);
inline const Pin PC2(GPIOC, 2);
inline const Pin PC3(GPIOC, 3);
inline const Pin PC4(GPIOC, 4);
inline const Pin PC5(GPIOC, 5);
inline const Pin PC6(GPIOC, 6);
inline const Pin PC7(GPIOC, 7);
inline const Pin PC8(GPIOC, 8);
inline const Pin PC9(GPIOC, 9);
inline const Pin PC10(GPIOC, 10);
inline const Pin PC11(GPIOC, 11);
inline const Pin PC12(GPIOC, 12);
inline const Pin PC13(GPIOC, 13);
inline const Pin PC14(GPIOC, 14);
inline const Pin PC15(GPIOC, 15);

inline const Pin PD0(GPIOD, 0);
inline const Pin PD1(GPIOD, 1);
inline const Pin PD2(GPIOD, 2);
inline const Pin PD3(GPIOD, 3);
inline const Pin PD4(GPIOD, 4);
inline const Pin PD5(GPIOD, 5);
inline const Pin PD6(GPIOD, 6);
inline const Pin PD7(GPIOD, 7);
inline const Pin PD8(GPIOD, 8);
inline const Pin PD9(GPIOD, 9);
inline const Pin PD10(GPIOD, 10);
inline const Pin PD11(GPIOD, 11);
inline const Pin PD12(GPIOD, 12);
inline const Pin PD13(GPIOD, 13);
inline const Pin PD14(GPIOD, 14);
inline const Pin PD15(GPIOD, 15);

inline const Pin PE0(GPIOE, 0);
inline const Pin PE1(GPIOE, 1);
inline const Pin PE2(GPIOE, 2);
inline const Pin PE3(GPIOE, 3);
inline const Pin PE4(GPIOE, 4);
inline const Pin PE5(GPIOE, 5);
inline const Pin PE6(GPIOE, 6);
inline const Pin PE7(GPIOE, 7);
inline const Pin PE8(GPIOE, 8);
inline const Pin PE9(GPIOE, 9);
inline const Pin PE10(GPIOE, 10);
inline const Pin PE11(GPIOE, 11);
inline const Pin PE12(GPIOE, 12);
inline const Pin PE13(GPIOE, 13);
inline const Pin PE14(GPIOE, 14);
inline const Pin PE15(GPIOE, 15);

inline const Pin PF0(GPIOF, 0);
inline const Pin PF1(GPIOF, 1);
inline const Pin PF2(GPIOF, 2);
inline const Pin PF3(GPIOF, 3);
inline const Pin PF4(GPIOF, 4);
inline const Pin PF5(GPIOF, 5);
inline const Pin PF6(GPIOF, 6);
inline const Pin PF7(GPIOF, 7);
inline const Pin PF8(GPIOF, 8);
inline const Pin PF9(GPIOF, 9);
inline const Pin PF10(GPIOF, 10);
inline const Pin PF11(GPIOF, 11);
inline const Pin PF12(GPIOF, 12);
inline const Pin PF13(GPIOF, 13);
inline const Pin PF14(GPIOF, 14);
inline const Pin PF15(GPIOF, 15);

inline const Pin PG0(GPIOG, 0);
inline const Pin PG1(GPIOG, 1);
inline const Pin PG2(GPIOG, 2);
inline const Pin PG3(GPIOG, 3);
inline const Pin PG4(GPIOG, 4);
inline const Pin PG5(GPIOG, 5);
inline const Pin PG6(GPIOG, 6);
inline const Pin PG7(GPIOG, 7);
inline const Pin PG8(GPIOG, 8);
inline const Pin PG9(GPIOG, 9);
inline const Pin PG10(GPIOG, 10);
inline const Pin PG11(GPIOG, 11);
inline const Pin PG12(GPIOG, 12);
inline const Pin PG13(GPIOG, 13);
inline const Pin PG14(GPIOG, 14);
inline const Pin PG15(GPIOG, 15);

inline const Pin PH0(GPIOH, 0);
inline const Pin PH1(GPIOH, 1);

#endif
