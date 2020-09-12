#ifndef _H_SHELL_
#define _H_SHELL_

#include "events.hpp"
#include "terminal.hpp"

namespace Shell {

const int MAX_MODULES = 4;
const int MAX_COMMAND_WORDS = 6;

enum CommandResult {
  SKIPPED,
  COMMAND_OK,
  INVALID_VALUE_FOR_VARIABLE,
  COMMAND_ERROR
};

class Module {
public:

  Module(const char *module_name) : name(module_name) { }

  virtual CommandResult set_variable(const char *name, const char *value) = 0;
  virtual CommandResult show_variable(const char *name) = 0;
  virtual CommandResult run_command(const char *cmd, int nargs, char *args[]) = 0;

private:

  const char *name;
};

class CommandShell;

class CoreModule : public Module {
public:

  CoreModule() : Module("core") { }

  virtual CommandResult set_variable(const char *name, const char *value);
  virtual CommandResult show_variable(const char *name);
  virtual CommandResult run_command(const char *cmd, int nargs, char *args[]);

  void setShell(CommandShell &shell) { sh = &shell; }

private:

  CommandShell *sh;
};

class CommandShell : public Events::Consumer {
public:
  CommandShell(TerminalInterface &terminal);
  virtual bool dispatch(const Events::Event &e) override;

  void process_command(char *buf);

  CommandResult set_variable(char *name, char *value);
  CommandResult show_variable(char *name);

  template<typename F>
  CommandResult process_boolean(const char *valstr, F setter);

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

bool parse(const char *s, bool &val);
bool parse(const char *s, int &val);

} // namespace Shell

#endif
