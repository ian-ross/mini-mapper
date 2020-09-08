#include "events_mock.hpp"

std::vector<Events::Event> MockEventWaiter::pending;

void MockEventWaiter::wait_for_event() {
  //
}
