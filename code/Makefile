# Top-level makefile for Linux-side test builds.

all: builddir build/testexe


# BASE gives a reference directory for relative paths pointing to the
# top-level directory containing the mk, hw and lib directories.
BASE=$(shell pwd)

# Optimise for debug for Linux test builds.
OPT=-g -Og

# Linux and common build definitions.
include $(BASE)/mk/linux.mk
include $(BASE)/mk/common.mk

# Test infrastructure sources: doctest main program, Linux-side fake
# STM32F767xx peripheral CMSIS definitions, test mocks.
TESTSRCS = \
  test/test_main.cpp \
  test/mocks/core_cm7_peripherals.c \
  test/mocks/stm32f767xx_peripherals.c \
  hw/system_stm32f7xx.c \
  $(wildcard test/mocks/*_mock.cpp)

# Test object files: convert extensions, convert paths to point into
# build directory.
TESTOBJS1 = $(patsubst %.cpp,%.o,$(TESTSRCS))
TESTOBJS2 = $(patsubst %.c,%.o,$(TESTOBJS1))
TESTOBJS = $(addprefix build/,$(TESTOBJS2))

# Don't use libmm.a library here because doctest tests are defined by
# test object initialisers, which won't get pulled in if they're only
# defined in a library. Instead list the TESTOBJS explicitly.
build/testexe: $(TESTOBJS) $(LIBOBJS)
	$(info ==> Linking: $@)
	@$(CXX) -std=c++17 -o $@ $^ $(LDFLAGS)

# Run doctest test program: output to console.
.PHONY: test
test: build/testexe
	@build/testexe
