#ifndef _H_EVENTS_
#define _H_EVENTS_

#include <stdint.h>
#include <array>

namespace Events {

// All known event types.

enum Tag {
  NO_EVENT = 0,
  SYSTICK,
  USART_RX_CHAR,
  USART_RX_OVERRUN,
  USART_TX_DMA_DONE,
  USART_TX_ERROR,
  USART_TX_OVERFLOW,
  TERMINAL_LINE_RECEIVED,
  TERMINAL_RX_OVERFLOW,
  TERMINAL_CANNOT_FLUSH,
};


// Events have a tag and an optional parameter, which we keep as a
// uint32_t for simplicity.

struct Event {
  Tag tag;
  uint32_t param;
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

  Manager(EventWaiter w);

  // Add an event consumer.
  void operator+=(Consumer &c);

  // Post a new event to the event queue.
  void post(Tag tag, uint32_t param = 0);

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

  // Function to wait for new hardware events.
  EventWaiter waiter;

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
// dispatch function.

class Consumer {
public:

  friend class Manager;

  Manager &mgr(void) { return *ev; }
  virtual bool dispatch(const Event &e) = 0;

private:

  Manager *ev;
};

}

#endif
