#ifndef _H_TERMINAL_MOCK_
#define _H_TERMINAL_MOCK_

#include "terminal.hpp"

#include "doctest.h"
#include "doctest/trompeloeil.hpp"

class MockTerminal : public trompeloeil::mock_interface<TerminalInterface> {
public:
  IMPLEMENT_MOCK1(set_interactive);
  IMPLEMENT_MOCK1(buffer);
  MAKE_MOCK1(print, void(const char *), override);
  MAKE_MOCK1(print, void(int), override);
  MAKE_MOCK1(println, void(const char *), override);
  MAKE_MOCK1(println, void(int), override);
  IMPLEMENT_MOCK0(flush);
  IMPLEMENT_MOCK1(error);
};

#endif
