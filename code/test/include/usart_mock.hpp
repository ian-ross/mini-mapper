#ifndef _H_USART_MOCK_
#define _H_USART_MOCK_

#include "events.hpp"

#include "doctest.h"
#include "doctest/trompeloeil.hpp"

class MockUSART : public trompeloeil::mock_interface<Events::Consumer> {
public:

  MockUSART() : trompeloeil::mock_interface<Events::Consumer>("(USART)") {}

  MAKE_MOCK1(dispatch, bool(const Events::Event &), override);
  MAKE_MOCK1(tx, void(char));
  MAKE_MOCK0(flush, void(void));
};

#endif
