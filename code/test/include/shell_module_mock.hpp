#ifndef _H_SHELL_MODULE_MOCK_
#define _H_SHELL_MODULE_MOCK_

#include "shell.hpp"

#include "doctest.h"
#include "doctest/trompeloeil.hpp"

class MockShellModule : public trompeloeil::mock_interface<Shell::Module> {
public:
  MockShellModule() : trompeloeil::mock_interface<Shell::Module>("(SHELL-MODULE)") { }

  IMPLEMENT_MOCK2(set_variable);
  IMPLEMENT_MOCK1(show_variable);
  IMPLEMENT_MOCK3(run_command);
};

#endif
