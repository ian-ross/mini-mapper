#include "events.hpp"
#include "events_mock.hpp"



//----------------------------------------------------------------------
//
//  TESTS
//

#include "doctest.h"
#include "doctest/trompeloeil.hpp"
#include "events_mock.hpp"

namespace trompeloeil {
  template <>
  inline void print(std::ostream& os, const Events::Event &e) {
    os << "Event{" << e.tag << "," << e.param << "}";
  }
}

TEST_CASE("event manager") {
  using trompeloeil::_;

  MockEventWaiter waiter;
  Events::Manager<MockEventWaiter::wait_for_event> ev;
  MockEventConsumer consumer;
  ev += consumer;

  SUBCASE("can add consumers") {
    CHECK(ev.consumer_count() == 1);
  }

  SUBCASE("can post events") {
    ev.post(Events::USART_RX_CHAR, 'c');
    CHECK(ev.pending_count() == 1);
  }

  SUBCASE("events are delivered") {
    ev.post(Events::USART_RX_CHAR, 'c');
    REQUIRE_CALL(consumer, dispatch(_)).
      RETURN(_1.tag == Events::USART_RX_CHAR);
    ev.drain();
    CHECK(ev.pending_count() == 0);
  }
}
