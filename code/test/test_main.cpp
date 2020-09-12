#include <iostream>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

void fatal(const char *msg, int *param) {
  std::cout << "FATAL: " << msg << std::endl;
  if (param != nullptr) {
    std::cout << "param = " << *param << std::endl;
  }
  std::exit(1);
}
