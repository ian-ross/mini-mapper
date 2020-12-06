#ifndef _H_EVENTS_
#define _H_EVENTS_

// The event management interface maintains a simple fixed size queue
// of events to be dispatched. These events are dispatched to "event
// consumers" from a naive event loop.
//
// The philosophy here is that events are *processed* cooperatively by
// event consumers driven from the event loop, while events are posted
// to the event queue either by the consumers (to pass information to
// other consumers) or by interrupt handlers that service hardware
// resources. In this setup, an interrupt handler does the minimum
// amount of work required to service the hardware it's associated
// with, then calls Events::Manager::post to put an event in the queue
// containing information that allows an event consumer to perform
// "bottom half" processing.

#include <stdint.h>
#include <array>

namespace Events {

// All known event types: for the moment this is a fixed set for
// simplicity.

enum Tag {
  NO_EVENT = 0,
  EVENT_LOOP_STARTED,
  SYSTICK,
  USART_INIT,
  USART_RX_CHAR,
  USART_RX_OVERRUN,
  USART_TX_DMA_DONE,
  USART_TX_ERROR,
  USART_TX_OVERFLOW,
  TERMINAL_INIT,
  TERMINAL_LINE_RECEIVED,
  TERMINAL_LINE_PROCESSED,
  TERMINAL_RX_OVERFLOW,
  TERMINAL_CANNOT_FLUSH,
  SHELL_INIT,
  ENCODER_CAPTURE,
  ENCODER_OVERCAPTURE,
};


// Events have a tag and two optional parameters, which we keep as a
// uint32_t for simplicity.

struct Event {
  Tag tag;
  uint32_t param1;
  uint32_t param2;

  bool operator==(const Event &other) {
    return tag == other.tag && param1 == other.param1 && param2 == other.param2;
  }
};


// A function that we can call to wait for events to come in. On the
// STM32, this is basically an inline WFE instruction, while for
// Linux-side testing, we have a function that can inject "fake"
// hardware events.

using EventWaiter = auto (*)(void) -> void;


// An event manager: this has a fixed size event queue, a fixed size
// set of event consumers and some way of waiting for new hardware
// events to come in.

class Consumer;

const int QUEUE_SIZE = 8;
const int MAX_CONSUMERS = 8;

class Manager {
public:

  // An event manager needs to be initialised with a function to wait
  // for new events.
  Manager(EventWaiter w);

  // Add an event consumer.
  void operator+=(Consumer &c);

  // Post a new event to the event queue.
  void post(Tag tag, uint32_t param1 = 0, uint32_t param2 = 0);

  // Consumer and pending event counts.
  int consumer_count(void) const { return nconsumers; }
  int pending_count(void) const { return nevents; }

  // Dispatch all pending events.
  void drain(void);

  // Wait for hardware events.
  void wait(void) { waiter(); }

  // Repeatedly drain the event queue and wait for new events.
  void loop(void);

private:

  void deliver(Event &e);

  // Function to wait for new hardware events.
  EventWaiter waiter;

  // Has the event loop been started?
  bool started = false;

  // Use std::array for event queue and consumers list to have a fixed
  // sized array that looks like an STL vector.
  std::array<Event, QUEUE_SIZE> queue;
  std::array<Consumer *, MAX_CONSUMERS> consumers;

  // Current sizes of event queue and consumers list.
  int nevents = 0;
  int nconsumers = 0;

  // Next insert position for event queue.
  int qpos = 0;

};


// An event consumer: basically just an interface wrapping an event
// dispatch function (but one that has access to the event manager
// it's associated with, since a common pattern is for a consumer to
// post more events from within its dispatch method).

class Consumer {
public:

  Consumer(const char *n) : n{n} {}
  const char *name(void) const { return n; }

  friend class Manager;
  virtual void dispatch(const Event &e) = 0;

protected:

  Manager *mgr;
  const char *n;
};

}

#endif
