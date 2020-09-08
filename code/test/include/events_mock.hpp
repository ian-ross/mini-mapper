#ifndef _H_EVENTS_MOCK_
#define _H_EVENTS_MOCK_

#include <vector>
#include "events.hpp"

#include "doctest.h"
#include "doctest/trompeloeil.hpp"

class MockEventWaiter {
public:

  MockEventWaiter() {}
  void add_event(Events::Tag tag, uint32_t param = 0);
  static void wait_for_event(void);

private:

  static std::vector<Events::Event> pending;
};

class MockEventConsumer : public trompeloeil::mock_interface<Events::Consumer> {
public:

  IMPLEMENT_MOCK1(dispatch);
};

#endif
