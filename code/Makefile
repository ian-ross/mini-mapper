include mk/linux.mk
include mk/common.mk

LIBSRCS = $(wildcard lib/*.cpp)
TESTSRCS = \
  test/test_main.cpp \
  test/mocks/core_cm7_peripherals.c \
  test/mocks/stm32f767xx_peripherals.c \
  $(wildcard test/mocks/*_mock.cpp)

LIBOBJS = $(addprefix build/,$(patsubst %.cpp,%.o,$(LIBSRCS)))
TESTOBJS = $(addprefix build/,$(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(TESTSRCS))))


all: builddir build/libmm.a build/testexe

.PRECIOUS: $(LIBOBJS)

build/libmm.a: build/libmm.a($(LIBOBJS))

build/testexe: $(TESTOBJS) $(LIBOBJS)
	$(info Linking: $@)
	@$(CXX) -std=c++17 -o $@ $^ $(LDFLAGS)

.PHONY: test
test: build/testexe
	@build/testexe

.PHONY: builddir
builddir:
	@$(MK) -p build

.PHONY: clean
clean:
	@$(RM) -fr build compile_commands.json

build/%.o: %.cpp
	$(info Compiling file: $<)
	@$(MK) -p $(dir $@)
	@$(CXX) -std=c++17 -MP -MMD -c -o $@ $< $(CXXFLAGS) $(INC_PATHS)

build/%.o: %.c
	$(info Compiling file: $<)
	@$(MK) -p $(dir $@)
	@$(CC) -MP -MMD -c -o $@ $< $(CFLAGS) $(INC_PATHS)
