#include <iostream>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

char fatal_msg[256] = "";
int fatal_param = 0;

void fatal(const char *msg, int *param) {
  strcpy(fatal_msg, msg);
  if (param != nullptr) fatal_param = *param;
}
