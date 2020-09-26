#ifndef _H_USART_MOCK_
#define _H_USART_MOCK_

#include "events.hpp"

#include "doctest.h"
#include "doctest/trompeloeil.hpp"

class MockUSART : public trompeloeil::mock_interface<Events::Consumer> {
public:

  MockUSART() : trompeloeil::mock_interface<Events::Consumer>("(USART)") {}

  MAKE_CONST_MOCK0(index, int(void));
  MAKE_MOCK1(dispatch, void(const Events::Event &), override);
  MAKE_MOCK1(tx, void(char));
  MAKE_MOCK0(flush, void(void));
};

#endif
