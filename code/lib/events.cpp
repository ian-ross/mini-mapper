#include "events.hpp"
#ifdef TEST
#include <iostream>
#endif


namespace Events {

Manager::Manager(EventWaiter w) : waiter{w} {
  queue.fill(Event{NO_EVENT, 0});
  consumers.fill(nullptr);
}

void Manager::operator+=(Consumer &c) {
  // TODO: ERROR CHECKING?
  consumers[nconsumers++] = &c;
  c.mgr = this;
}

void Manager::post(Tag tag, uint32_t param) {
#ifdef TEST
  if (debug) {
    std::cout << "Events::Manager::post tag=" << tag << "  param=" << param
              << std::endl;
  }
#endif

  if (nevents >= QUEUE_SIZE) {
    // TODO: ERROR CHECKING
  }
  queue[qpos].tag = tag;
  queue[qpos].param = param;
  qpos = (qpos + 1) % QUEUE_SIZE;
  nevents++;
}

  // Process queued events one by one, trying to dispatch to each
// consumer in turn. A consumer can "claim" the event by returning
// `true` from its `dispatch` method.
//
// TODO: THIS COULD BE MADE MORE EFFICIENT, BUT IT'S GOOD ENOUGH FOR
// NOW.

void Manager::drain(void) {
#ifdef TEST
  if (debug) {
    std::cout << "Events::Manager::drain  nevents=" << nevents << std::endl;
  }
#endif
  while (nevents > 0) {
    for (int i = 0; i < QUEUE_SIZE; ++i) {
      if (queue[i].tag != NO_EVENT) {
#ifdef TEST
        if (debug) {
          std::cout << "  i=" << i << " tag=" << queue[i].tag << std::endl;
        }
#endif
        bool consumed = false;
        for (int j = 0; !consumed && j < nconsumers; ++j) {
          if (consumers[j]) {
#ifdef TEST
            if (debug) {
              std::cout << "  Try " << consumers[j]->name() << std::endl;
            }
#endif
            if (consumers[j]->dispatch(queue[i])) {
              consumed = true;
            }
          }
        }
        if (!consumed) {
          // TODO: ERROR -- EVENT NOT CONSUMED
        }
        queue[i].tag = NO_EVENT;
        nevents--;
      }
    }
  }
}


// Event loop: drain the event queue, wait for more events, repeat
// forever.

void Manager::loop(void) {
  while (true) {
    drain();
    wait();
  }
}


}

//----------------------------------------------------------------------
//
//  TESTS
//

#ifdef TEST

#include "doctest.h"
#include "doctest/trompeloeil.hpp"
#include "events_mock.hpp"

namespace trompeloeil {
  template <>
  inline void print(std::ostream& os, const Events::Event &e) {
    os << "Event{" << e.tag << "," << e.param << "}";
  }
}

TEST_CASE("Event management") {
  using trompeloeil::_;

  MockEventWaiter waiter;
  Events::Manager ev(MockEventWaiter::wait_for_event);
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

#endif
