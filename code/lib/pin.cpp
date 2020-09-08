#include "pin.hpp"

void Pin::Output(GPIOSpeed speed, GPIOOutputType type, GPIOPUPD pupd) const {
  EnableClock();
  MODIFY_REG(port->OSPEEDR, 0x03 << (2 * pin), (int)speed << (2 * pin));
  MODIFY_REG(port->OTYPER, 0x01 << pin, (int)type << pin);
  MODIFY_REG(port->PUPDR, 0x03 << (2 * pin), (int)pupd << (2 * pin));
  MODIFY_REG(port->MODER, 0x03 << (2 * pin), 0x01 << (2 * pin));
}

void Pin::Input(GPIOSpeed speed) const {
  EnableClock();
  MODIFY_REG(port->OSPEEDR, 0x03 << (2 * pin), (int)speed << (2 * pin));
  MODIFY_REG(port->MODER, 0x03 << (2 * pin), 0x00 << (2 * pin));
}

void Pin::Alternate(GPIOAF af) const {
  EnableClock();
  MODIFY_REG(port->MODER, 0x03 << (2 * pin), 0x02 << (2 * pin));
  MODIFY_REG(port->AFR[pin / 8], 0x0F << 4 * (pin % 8), af << 4 * (pin % 8));
  int wait = 0;
  while (wait < 10) { ++wait; }
}


//----------------------------------------------------------------------
//
//  TESTS
//

#include "doctest.h"

TEST_CASE("GPIOs work") {
  init_peripheral_clocks();
  init_mock_gpios();

  SUBCASE("mock GPIO initialisation") {
    CHECK(((GPIOA->MODER & GPIO_MODER_MODER15_Msk) >> GPIO_MODER_MODER15_Pos) == 0x02);
    CHECK(READ_BIT(GPIOC->OTYPER, GPIO_OTYPER_OT11) == 0x00);
    CHECK(((GPIOB->OSPEEDR & GPIO_OSPEEDR_OSPEEDR3_Msk) >> GPIO_OSPEEDR_OSPEEDR3_Pos) == 0x03);
  }

  SUBCASE("GPIO output setup") {
    PD8.Output(GPIO_SPEED_VERY_HIGH, GPIO_TYPE_OPEN_DRAIN, GPIO_PUPD_PULL_UP);
    CHECK(READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN) == RCC_AHB1ENR_GPIODEN);
    CHECK(((GPIOD->OSPEEDR & (0x03 << 16)) >> 16) == GPIO_SPEED_VERY_HIGH);
    CHECK((READ_BIT(GPIOD->OTYPER, 1 << 8) >> 8) == GPIO_TYPE_OPEN_DRAIN);
    CHECK(((GPIOD->MODER & (0x03 << 16)) >> 16) == 0x01);
  }

  SUBCASE("GPIO input setup") {
    PC9.Input(GPIO_SPEED_HIGH);
    CHECK(READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN) == RCC_AHB1ENR_GPIOCEN);
    CHECK(((GPIOC->OSPEEDR & (0x03 << 18)) >> 18) == GPIO_SPEED_HIGH);
  }

  SUBCASE("GPIO alternate function setup") {
    PA7.Alternate(GPIO_AF_5);
    CHECK(READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN) == RCC_AHB1ENR_GPIOAEN);
    CHECK(((GPIOA->MODER & (0x03 << 14)) >> 14) == 0x02);
    CHECK(((GPIOA->AFR[0] & (0x0F << 28)) >> 28) == 0x05);
    CHECK(GPIOA->AFR[1] == 0x00);
  }

  SUBCASE("GPIO output set/reset/toggle") {
    PA6.Output(GPIO_SPEED_VERY_HIGH, GPIO_TYPE_OPEN_DRAIN, GPIO_PUPD_PULL_UP);
    PA6.Set();
    CHECK((READ_BIT(GPIOA->ODR, 1 << 6) >> 6) == 0x01);
    PA6.Reset();
    CHECK((READ_BIT(GPIOA->ODR, 1 << 6) >> 6) == 0x00);
    PA6.Toggle();
    CHECK((READ_BIT(GPIOA->ODR, 1 << 6) >> 6) == 0x01);
    PA6.Toggle();
    CHECK((READ_BIT(GPIOA->ODR, 1 << 6) >> 6) == 0x00);
  }
}
