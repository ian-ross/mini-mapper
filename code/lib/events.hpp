#ifndef _H_EVENTS_
#define _H_EVENTS_

#include <stdint.h>
#include <array>
#include <iostream>

namespace Events {

enum Tag {
  NO_EVENT = 0,
  USART_RX_CHAR,
  USART_RX_OVERRUN,
  USART_TX_DMA_DONE,
  USART_TX_ERROR,
  TERMINAL_LINE_RECEIVED,
  TERMINAL_RX_OVERFLOW,
  TERMINAL_CANNOT_FLUSH,
};

struct Event {
  Tag tag;
  uint32_t param;
};

using EventWaiter = auto (*)(void) -> void;

// EventWaiter RealWaiter = __WFE;

class Consumer {
  public:
  virtual bool dispatch(const Event &e) = 0;
};

template<EventWaiter W, int QUEUE_SIZE = 8, int MAX_CONSUMERS = 8>
class Manager {
  public:

  Manager();
  void operator+=(Consumer &c);
  int consumer_count(void) const { return nconsumers; }
  void post(Tag tag, uint32_t param = 0);
  int pending_count(void) const { return nevents; }
  void drain(void);
  void wait(void) { W(); }
  void loop(void);

  private:

  std::array<Event, QUEUE_SIZE> queue;
  int qpos = 0;
  int nevents = 0;
  std::array<Consumer *, MAX_CONSUMERS> consumers;
  int nconsumers = 0;
};

template<EventWaiter W, int QUEUE_SIZE, int MAX_CONSUMERS>
Manager<W, QUEUE_SIZE, MAX_CONSUMERS>::Manager() {
  queue.fill(Event{NO_EVENT, 0});
  consumers.fill(nullptr);
}

template<EventWaiter W, int QUEUE_SIZE, int MAX_CONSUMERS>
void Manager<W, QUEUE_SIZE, MAX_CONSUMERS>::operator+=(Consumer &c) {
  consumers[nconsumers++] = &c;
}

template<EventWaiter W, int QUEUE_SIZE, int MAX_CONSUMERS>
void Manager<W, QUEUE_SIZE, MAX_CONSUMERS>::post(Tag tag, uint32_t param) {
  if (nevents >= QUEUE_SIZE) {
    // ERROR!
  }
  queue[qpos].tag = tag;
  queue[qpos].param = param;
  qpos = (qpos + 1) % QUEUE_SIZE;
  nevents++;
}

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

template<EventWaiter WAIT, int QUEUE_SIZE, int MAX_CONSUMERS>
void Manager<WAIT, QUEUE_SIZE, MAX_CONSUMERS>::loop(void) {
  while (true) {
    drain();
    wait();
  }
}

}

#endif
