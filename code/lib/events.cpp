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
    return;
  }
  consumers[nconsumers++] = &c;
  c.mgr = this;
}


// Post a new event to the queue, which we treat as a circular buffer.

void Manager::post(Tag tag, uint32_t param) {
  if (!started) {
    if (tag != SYSTICK) fatal("event posted before event loop started");
    return;
  }
  if (nevents >= QUEUE_SIZE) {
    fatal("too many pending events");
    return;
  }
  queue[qpos].tag = tag;
  queue[qpos].param = param;
  qpos = (qpos + 1) % QUEUE_SIZE;
  nevents++;
}


// Deliver a single event.

void Manager::deliver(Event &e) {
  // Try the consumers in turn to see who wants to process this
  // event...
  for (int j = 0; j < nconsumers; ++j) {
    if (consumers[j] != nullptr) {
      consumers[j]->dispatch(e);
    }
  }
}


// Process queued events one by one, trying to dispatch to each
// consumer in turn. A consumer can "claim" the event by returning
// `true` from its `dispatch` method.
//
// TODO: THIS COULD BE MADE MORE EFFICIENT, BUT IT'S GOOD ENOUGH FOR
// NOW.

void Manager::drain(void) {
  if (!started) {
    started = true;
    Event start{EVENT_LOOP_STARTED, 0};
    deliver(start);
  }

  for (int i = qpos; nevents > 0; i = (i + 1) % QUEUE_SIZE) {
    if (queue[i].tag != NO_EVENT) {
      // Deliver event and empty the slot in the event queue.
      deliver(queue[i]);
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

  SUBCASE("cannot post events before event loop started") {
    ev.post(Events::USART_RX_CHAR, 'c');
    CHECK(ev.pending_count() == 0);
  }

  SUBCASE("events are delivered") {
    REQUIRE_CALL(consumer, dispatch(_));
    ev.drain();
    ev.post(Events::USART_RX_CHAR, 'c');
    REQUIRE_CALL(consumer, dispatch(_));
    ev.drain();
    CHECK(ev.pending_count() == 0);
  }
}

#endif
