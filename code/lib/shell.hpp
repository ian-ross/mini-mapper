#ifndef _H_SHELL_
#define _H_SHELL_

#include "events.hpp"
#include "terminal.hpp"

namespace Shell {

// Fixed maximum number of command shell modules that can be added to
// a CommandShell instance.
const int MAX_MODULES = 4;

// Maximum number of words allowed in a command (i.e. one more than
// the maximum number of arguments.)
const int MAX_COMMAND_WORDS = 6;

// Return type for command processors:
enum CommandResult {
  SKIPPED,                      // Didn't process command.
  COMMAND_OK,                   // Processed OK.
  INVALID_VALUE_FOR_VARIABLE,   // Bad value for variable.
  COMMAND_ERROR                 // Miscellaneous command error.
};


// Class representing a self-contained set of commands and variables
// implementing some set of functionality to be drive from a command
// shell.

class Module {
public:

  Module(const char *module_name) : name(module_name) { }

  virtual CommandResult set_variable
    (const char *name, const char *value) = 0;
  virtual CommandResult show_variable(const char *name) = 0;
  virtual CommandResult run_command
    (const char *cmd, int nargs, char *args[]) = 0;

private:

  const char *name;
};


class CommandShell;


// Core shell module implementing the "set" and "show" commands for
// handing off variable manipulation to other shell modules.

class CoreModule : public Module {
public:

  CoreModule() : Module("core") { }

  virtual CommandResult set_variable(const char *name, const char *value) {
    return SKIPPED;
  }
  virtual CommandResult show_variable(const char *name) {
    return SKIPPED;
  }
  virtual CommandResult run_command(const char *cmd, int nargs, char *args[]);

  void setShell(CommandShell &shell) { sh = &shell; }

private:

  CommandShell *sh;
};


// Main command shell class. This is driven from a terminal
// (represented by a TerminalInterface) and processes one input line
// at a time as a command. Command functionality is implmented by
// shell "modules" that are attached ot the CommandShell.

class CommandShell : public Events::Consumer {
public:
  CommandShell(TerminalInterface &terminal);
  virtual bool dispatch(const Events::Event &e) override;

  void process_command(char *buf);

  CommandResult set_variable(char *name, char *value);
  CommandResult show_variable(char *name);

  void operator+=(Module &mod);
  TerminalInterface *terminal(void) { return term; }

  int module_count(void) const { return nmodules; }

private:

  bool handle_result(CommandResult cr);

  TerminalInterface *term;
  CoreModule core;
  Module *modules[MAX_MODULES];
  int nmodules = 0;
};


// Simple parsers for variable and parameter values.
bool parse(const char *s, bool &val);
bool parse(const char *s, int &val);

} // namespace Shell

#endif
