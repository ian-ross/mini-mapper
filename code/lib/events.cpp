#include "errors.hpp"
#include "events.hpp"


namespace Events {

// An event manager maintains a queue of events and a set of event
// consumers. The constructor here initialises both of these to all
// placeholder values.

Manager::Manager(EventWaiter w) : waiter{w} {
  queue.fill(Event{NO_EVENT, 0});
  consumers.fill(nullptr);
}


// Add a consumer.

void Manager::operator+=(Consumer &c) {
  if (nconsumers >= MAX_CONSUMERS) {
    fatal("too many event consumers");
  }
  consumers[nconsumers++] = &c;
  c.mgr = this;
}


// Post a new event to the queue, which we treat as a circular buffer.

void Manager::post(Tag tag, uint32_t param) {
  if (nevents >= QUEUE_SIZE) {
    fatal("too many pending events");
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
  for (int i = qpos; nevents > 0; i = (i + 1) % QUEUE_SIZE) {
    if (queue[i].tag != NO_EVENT) {
      bool consumed = false;

      // Try the consumers in turn to see who wants to process this
      // event...
      for (int j = 0; !consumed && j < nconsumers; ++j) {
        if (consumers[j] != nullptr) {
          if (consumers[j]->dispatch(queue[i])) {
            consumed = true;
          }
        }
      }

      // Make sure that all events that are posted get consumed
      // (except for SysTick, which can be used by multiple
      // consumers).
      if (!consumed && queue[i].tag != SYSTICK) {
        int tag = queue[i].tag;
        fatal("event not consumed", &tag);
      }

      // Empty the slot in the event queue.
      queue[i].tag = NO_EVENT;
      nevents--;
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
    REQUIRE_CALL(consumer, dispatch(_))
      .RETURN(_1.tag == Events::USART_RX_CHAR);
    ev.drain();
    CHECK(ev.pending_count() == 0);
  }
}

#endif
