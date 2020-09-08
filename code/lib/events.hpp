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


// An event consumer: basically just an interface wrapping an event
// dispatch function.

class Consumer {
  public:
  virtual bool dispatch(const Event &e) = 0;
};


// An event manager: this has a fixed size event queue, a fixed size
// set of event consumers and some way of waiting for new hardware
// events to come in.

template<EventWaiter W, int QUEUE_SIZE = 8, int MAX_CONSUMERS = 8>
class Manager {
public:

  Manager();

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
  void wait(void) { W(); }

  // Repeatedly drain the event queue and wait for new events.
  void loop(void);

private:

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


template<EventWaiter W, int QUEUE_SIZE, int MAX_CONSUMERS>
Manager<W, QUEUE_SIZE, MAX_CONSUMERS>::Manager() {
  queue.fill(Event{NO_EVENT, 0});
  consumers.fill(nullptr);
}


template<EventWaiter W, int QUEUE_SIZE, int MAX_CONSUMERS>
void Manager<W, QUEUE_SIZE, MAX_CONSUMERS>::operator+=(Consumer &c) {
  // TODO: ERROR CHECKING?
  consumers[nconsumers++] = &c;
}


template<EventWaiter W, int QUEUE_SIZE, int MAX_CONSUMERS>
void Manager<W, QUEUE_SIZE, MAX_CONSUMERS>::post(Tag tag, uint32_t param) {
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

template<EventWaiter WAIT, int QUEUE_SIZE, int MAX_CONSUMERS>
void Manager<WAIT, QUEUE_SIZE, MAX_CONSUMERS>::drain(void) {
  while (nevents > 0) {
    for (int i = 0; i < QUEUE_SIZE; ++i) {
      if (queue[i].tag != NO_EVENT) {
        for (auto consumer: consumers) {
          if (consumer->dispatch(queue[i])) {
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

template<EventWaiter WAIT, int QUEUE_SIZE, int MAX_CONSUMERS>
void Manager<WAIT, QUEUE_SIZE, MAX_CONSUMERS>::loop(void) {
  while (true) {
    drain();
    wait();
  }
}

}

#endif
