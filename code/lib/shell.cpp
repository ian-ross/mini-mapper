#include <cctype>
#include <cstring>

#include "shell.hpp"

namespace Shell {

CommandShell::CommandShell(TerminalInterface &t) :
  Events::Consumer("SHELL"), term(&t) {
  core.setShell(*this);
  *this += core;
}

bool CommandShell::dispatch(const Events::Event &e) {
  if (e.tag != Events::TERMINAL_LINE_RECEIVED) {
    return false;
  }

  process_command(term->buffer((TerminalRXBuffer)e.param));
  mgr->post(Events::TERMINAL_LINE_PROCESSED);
  return true;
}

void CommandShell::process_command(char *buf) {
  // Split command buffer into words.
  char *words[MAX_COMMAND_WORDS];
  int nwords = 0;
  bool in_word = false;
  for (char *p = buf; *p; ++p) {
    if (in_word) {
      if (*p == ' ') {
        *p = '\0';
        in_word = false;
      }
    } else {
      if (*p != ' ') {
        words[nwords++] = p;
        in_word = true;
      }
    }
  }

  bool processed = false;
  for (int i = 0; i < nmodules; ++i) {
    CommandResult res = modules[i]->run_command(words[0], nwords - 1, &words[1]);
    processed = handle_result(res);
    if (processed) break;
  }
  if (!processed) term->error("SHUN");
}

CommandResult CommandShell::set_variable(char *name, char *value) {
  bool processed = false;
  for (int i = 0; i < nmodules; ++i) {
    CommandResult res = modules[i]->set_variable(name, value);
    processed = handle_result(res);
    if (processed) break;
  }
  if (!processed) term->error("SHUV");
  return COMMAND_OK;
}


CommandResult CommandShell::show_variable(char *name) {
  bool processed = false;
  for (int i = 0; i < nmodules; ++i) {
    CommandResult res = modules[i]->show_variable(name);
    processed = handle_result(res);
    if (processed) break;
  }
  if (!processed) term->error("SHUV");
  return COMMAND_OK;
}

bool CommandShell::handle_result(CommandResult cr) {
  switch (cr) {
  case SKIPPED:
    // Do nothing: go to the next potential processor.
    return false;

  case COMMAND_OK:
    // Command processed.
    return true;
    break;

  case INVALID_VALUE_FOR_VARIABLE:
    term->error("SHIV");
    return true;
    break;

  case COMMAND_ERROR:
    term->error("SHER");
    return true;
    break;
  }
  return false;
}

void CommandShell::operator+=(Module &mod) {
  if (nmodules >= MAX_MODULES) {
    term->error("SHMM");
    return;
  }
  modules[nmodules++] = &mod;
}

template<typename F>
CommandResult CommandShell::process_boolean(const char *valstr, F setter) {
  bool val;
  if (parse(valstr, val)) {
    return setter(val);
  } else {
    return INVALID_VALUE_FOR_VARIABLE;
  }
}

CommandResult CoreModule::set_variable(const char *name, const char *value) {
  if (!strcmp(name, "interactive")) {
    return sh->process_boolean
      (value, [&](bool v) {
        sh->terminal()->set_interactive(v); return COMMAND_OK;
      });
  }
  return SKIPPED;
}

CommandResult CoreModule::show_variable(const char *name) {
  if (!strcmp(name, "interactive")) {
  }
  return SKIPPED;
}

CommandResult CoreModule::run_command(const char *cmd, int nargs, char *args[]) {
  if (!strcmp(cmd, "set")) {
    if (nargs != 2) return COMMAND_ERROR;
    return sh->set_variable(args[0], args[1]);
  }
  if (!strcmp(cmd, "show")) {
    if (nargs != 1)
      return COMMAND_ERROR;
    return sh->show_variable(args[0]);
  }
  return SKIPPED;
}

bool parse(const char *s, bool &val)
{
  if (!strcmp(s, "true") || !strcmp(s, "on")) {
    val = true;
    return true;
  } else if (!strcmp(s, "false") || !strcmp(s, "off")) {
    val = false;
    return true;
  }
  return false;
}

bool parse(const char *s, int &val) {
  int sign = 1;
  if (*s == '-') {
    sign = -1;
    ++s;
  }
  for (val = 0; *s != '\0' && isdigit(*s); ++s) {
    val = val * 10 + *s - '0';
  }
  val *= sign;
  return *s == '\0';
}

} // namespace Shell

//----------------------------------------------------------------------
//
//  TESTS
//

#ifdef TEST

#include <iostream>
#include "doctest.h"
#include "doctest/trompeloeil.hpp"
#include "events_mock.hpp"
#include "terminal_mock.hpp"
#include "shell_module_mock.hpp"

TEST_CASE("Command shell") {
  using trompeloeil::_;

  MockTerminal terminal;
  Shell::CommandShell shell(terminal);
  MockShellModule module;
  MockEventWaiter waiter;
  Events::Manager ev(MockEventWaiter::wait_for_event);
  ev += shell;

  SUBCASE("can add shell modules") {
    shell += module;
    CHECK(shell.module_count() == 2);
  }

  SUBCASE("commands get dispatched to correct module") {
    shell += module;

    char cmd[TERMINAL_RX_BUFSIZE] = "test";
    ALLOW_CALL(terminal, buffer(_)).RETURN((char *)&cmd[0]);
    ev.post(Events::TERMINAL_LINE_RECEIVED, TERMINAL_BUFFER_0);

    REQUIRE_CALL(module, run_command(_, 0, _))
      .WITH(strcmp(_1, "test") == 0)
      .RETURN(Shell::COMMAND_OK);
    ev.drain();
  }

  SUBCASE("command arguments are passed to module correctly") {
    shell += module;

    char cmd[TERMINAL_RX_BUFSIZE] = "test abc 123";
    ALLOW_CALL(terminal, buffer(_)).RETURN((char *)&cmd[0]);
    ev.post(Events::TERMINAL_LINE_RECEIVED, TERMINAL_BUFFER_0);

    REQUIRE_CALL(module, run_command(_, 2, _))
      .WITH(strcmp(_1, "test") == 0)
      .WITH(strcmp(_3[0], "abc") == 0)
      .WITH(strcmp(_3[1], "123") == 0)
      .RETURN(Shell::COMMAND_OK);
    ev.drain();
  }

  SUBCASE("unknown commands generate an error") {
    shell += module;

    char cmd[TERMINAL_RX_BUFSIZE] = "unknown";
    ALLOW_CALL(terminal, buffer(_)).RETURN((char *)&cmd[0]);
    ev.post(Events::TERMINAL_LINE_RECEIVED, TERMINAL_BUFFER_0);

    ALLOW_CALL(module, run_command(_, _, _)).RETURN(Shell::SKIPPED);

    REQUIRE_CALL(terminal, error(_)).WITH(strcmp(_1, "SHUN") == 0);
    ev.drain();
  }

  SUBCASE("variable value parsing works (boolean)") {
    bool chk;
    CHECK((Shell::parse("true", chk) && chk));
    CHECK((Shell::parse("false", chk) && !chk));
    CHECK(!Shell::parse("wrong", chk));
  }

  SUBCASE("variable value parsing works (integer)") {
    int chk;
    CHECK((Shell::parse("123", chk) && chk == 123));
    CHECK((Shell::parse("-10", chk) && chk == -10));
    CHECK(!Shell::parse("wrong", chk));
  }

  SUBCASE("variable setting works") {
    shell += module;

    char cmd[TERMINAL_RX_BUFSIZE] = "set test-var true";
    ALLOW_CALL(terminal, buffer(_)).RETURN((char *)&cmd[0]);
    ev.post(Events::TERMINAL_LINE_RECEIVED, TERMINAL_BUFFER_0);

    REQUIRE_CALL(module, set_variable(_, _))
      .WITH(strcmp(_1, "test-var") == 0)
      .WITH(strcmp(_2, "true") == 0)
      .RETURN(Shell::COMMAND_OK);
    ev.drain();
  }

  SUBCASE("setting unknown variable generates an error") {
    shell += module;

    char cmd[TERMINAL_RX_BUFSIZE] = "set test-var true";
    ALLOW_CALL(terminal, buffer(_)).RETURN((char *)&cmd[0]);
    ev.post(Events::TERMINAL_LINE_RECEIVED, TERMINAL_BUFFER_0);

    REQUIRE_CALL(module, set_variable(_, _)).RETURN(Shell::SKIPPED);
    REQUIRE_CALL(terminal, error(_)).WITH(strcmp(_1, "SHUV") == 0);
    ev.drain();
  }

  SUBCASE("showing variables works") {
    shell += module;

    char cmd[TERMINAL_RX_BUFSIZE] = "show test-var";
    ALLOW_CALL(terminal, buffer(_)).RETURN((char *)&cmd[0]);
    ev.post(Events::TERMINAL_LINE_RECEIVED, TERMINAL_BUFFER_0);

    REQUIRE_CALL(module, show_variable(_)).RETURN(Shell::COMMAND_OK);
    ev.drain();
  }
}

#endif
