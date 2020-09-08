#include "events.hpp"

namespace Events {

Manager::Manager(EventWaiter w) : waiter{w} {
  queue.fill(Event{NO_EVENT, 0});
  consumers.fill(nullptr);
}

void Manager::operator+=(Consumer &c) {
  // TODO: ERROR CHECKING?
  consumers[nconsumers++] = &c;
  c.ev = this;
}

void Manager::post(Tag tag, uint32_t param) {
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
  while (nevents > 0) {
    for (int i = 0; i < QUEUE_SIZE; ++i) {
      if (queue[i].tag != NO_EVENT) {
        for (auto consumer: consumers) {
          if (consumer && consumer->dispatch(queue[i])) {
            break;
          }
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

TEST_CASE("event manager") {
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
