#include "pin.hpp"

static void enable_clock(GPIO_TypeDef *port) {
  // Enable GPIO clock: AHB1ENR bits are one per port, starting from
  // zero, and the port addresses are every 0x0400 starting at the
  // base address.
  //
  // ** Note that this relies on these things being laid out in memory
  // ** the same way for Linux-side tests as on the STM32!!! See
  // ** test/mocks/stm32f767xx_peripherals.c for the details of how
  // ** this is done.
  uint32_t mask =
    1 << (((uintptr_t)port - (uintptr_t)AHB1PERIPH_BASE) / 0x0400UL);
  RCC->AHB1ENR |= mask;
}

void Pin::output(GPIOSpeed speed, GPIOOutputType type, GPIOPUPD pupd) const {
  enable_clock(_port);
  MODIFY_REG(_port->OSPEEDR, 0x03 << (2 * _pin), (int)speed << (2 * _pin));
  MODIFY_REG(_port->OTYPER, 0x01 << _pin, (int)type << _pin);
  MODIFY_REG(_port->PUPDR, 0x03 << (2 * _pin), (int)pupd << (2 * _pin));
  MODIFY_REG(_port->MODER, 0x03 << (2 * _pin), 0x01 << (2 * _pin));
}

void Pin::input(GPIOSpeed speed) const {
  enable_clock(_port);
  MODIFY_REG(_port->OSPEEDR, 0x03 << (2 * _pin), (int)speed << (2 * _pin));
  MODIFY_REG(_port->MODER, 0x03 << (2 * _pin), 0x00 << (2 * _pin));
}

void Pin::analog(void) const {
  enable_clock(_port);
  MODIFY_REG(_port->MODER, 0x03 << (2 * _pin), 0x03 << (2 * _pin));
}

void Pin::alternate(GPIOAF af) const {
  enable_clock(_port);
  MODIFY_REG(_port->MODER, 0x03 << (2 * _pin), 0x02 << (2 * _pin));
  MODIFY_REG(_port->AFR[_pin / 8], 0x0F << 4 * (_pin % 8), af << 4 * (_pin % 8));
}


//----------------------------------------------------------------------
//
//  TESTS
//

#ifdef TEST

#include "doctest.h"

TEST_CASE("GPIO abstraction") {
  init_peripheral_clocks();
  init_mock_gpios();

  SUBCASE("mock GPIO initialisation") {
    CHECK(((GPIOA->MODER & GPIO_MODER_MODER15_Msk) >>
           GPIO_MODER_MODER15_Pos) == 0x02);
    CHECK(READ_BIT(GPIOC->OTYPER, GPIO_OTYPER_OT11) == 0x00);
    CHECK(((GPIOB->OSPEEDR & GPIO_OSPEEDR_OSPEEDR3_Msk) >>
           GPIO_OSPEEDR_OSPEEDR3_Pos) == 0x03);
  }

  SUBCASE("GPIO output setup") {
    PD8.output(GPIO_SPEED_VERY_HIGH, GPIO_TYPE_OPEN_DRAIN, GPIO_PUPD_PULL_UP);
    CHECK(READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN) == RCC_AHB1ENR_GPIODEN);
    CHECK(((GPIOD->OSPEEDR & (0x03 << 16)) >> 16) == GPIO_SPEED_VERY_HIGH);
    CHECK((READ_BIT(GPIOD->OTYPER, 1 << 8) >> 8) == GPIO_TYPE_OPEN_DRAIN);
    CHECK(((GPIOD->MODER & (0x03 << 16)) >> 16) == 0x01);
  }

  SUBCASE("GPIO input setup") {
    PC9.input(GPIO_SPEED_HIGH);
    CHECK(READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN) == RCC_AHB1ENR_GPIOCEN);
    CHECK(((GPIOC->OSPEEDR & (0x03 << 18)) >> 18) == GPIO_SPEED_HIGH);
  }

  SUBCASE("GPIO alternate function setup") {
    PA7.alternate(GPIO_AF_5);
    CHECK(READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN) == RCC_AHB1ENR_GPIOAEN);
    CHECK(((GPIOA->MODER & (0x03 << 14)) >> 14) == 0x02);
    CHECK(((GPIOA->AFR[0] & (0x0F << 28)) >> 28) == 0x05);
    CHECK(GPIOA->AFR[1] == 0x00);
  }

  SUBCASE("GPIO output set/reset/toggle") {
    PA6.output(GPIO_SPEED_VERY_HIGH, GPIO_TYPE_OPEN_DRAIN, GPIO_PUPD_PULL_UP);
    PA6.set();
    CHECK((READ_BIT(GPIOA->ODR, 1 << 6) >> 6) == 0x01);
    PA6.reset();
    CHECK((READ_BIT(GPIOA->ODR, 1 << 6) >> 6) == 0x00);
    PA6.toggle();
    CHECK((READ_BIT(GPIOA->ODR, 1 << 6) >> 6) == 0x01);
    PA6.toggle();
    CHECK((READ_BIT(GPIOA->ODR, 1 << 6) >> 6) == 0x00);
  }
}

#endif
