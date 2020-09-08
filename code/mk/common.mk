# Build definitions common to Linux-side test builds and application
# cross-compilation builds. (Trying to keep as many things common
# between the two platforms as possible.)

# Basic tools.
MK := mkdir
RM := rm -rf

# Preprocessor constants shared among all tools: processor definition
# (used to select appropriate CMSIS definitions), and HSE input clock
# frequency.
COMMON_FLAGS += -DSTM32F767xx -DHSE_VALUE=8000000

# Common include paths: hardware definitions plus library code.
INCS += $(BASE)/hw $(BASE)/lib
INC_FLAGS := $(addprefix -I,$(INCS))

# Common GCC flags for all compilation tools.
COMMON_CFLAGS = \
  -ffunction-sections -fdata-sections \
  -fno-strict-aliasing -fno-builtin -fshort-enums

# Add common flags and include paths to tool-specific flags.
CFLAGS += $(OPT) $(COMMON_FLAGS) $(COMMON_CFLAGS) $(INC_FLAGS)
CXXFLAGS += $(OPT) $(COMMON_FLAGS) $(COMMON_CFLAGS) $(INC_FLAGS)
LDFLAGS += $(COMMON_FLAGS)

# Library sources: all C++.
LIBSRCS = $(notdir $(wildcard $(BASE)/lib/*.cpp))

# Library object files: fix file extensions, modify object paths to
# point into build directory.
LIBOBJS1 = $(patsubst %.cpp,%.o,$(LIBSRCS))
LIBOBJS = $(addprefix build/lib/,$(LIBOBJS1))

# Derive application object files from sources: fix file extensions,
# modify object paths to point into build directory.
OBJS1 = $(subst $(BASE)/,,$(SRCS))
OBJS2 = $(patsubst %.cpp,%.o,$(OBJS1))
OBJS3 = $(patsubst %.c,%.o,$(OBJS2))
OBJS4 = $(patsubst %.s,%.o,$(OBJS3))
OBJS = $(addprefix build/,$(OBJS4))

.PRECIOUS: $(LIBOBJS) $(OBJS)

# Application library.
build/libmm.a: build/libmm.a($(LIBOBJS))


# Utility rules.

.PHONY: builddir
builddir:
	@$(MK) -p build

.PHONY: clean
clean:
	@$(RM) -fr build compile_commands.json


# Standard compilation rules.

build/%.o: %.cpp
	$(info ==> Compiling file: $(notdir $<))
	@$(MK) -p $(dir $@)
	@$(CXX) -std=c++17 -MP -MMD -c -o $@ $< $(CXXFLAGS) $(INC_PATHS)

build/%.o: $(BASE)/%.cpp
	$(info ==> Compiling file: $(notdir $<))
	@$(MK) -p $(dir $@)
	@$(CXX) -std=c++17 -MP -MMD -c -o $@ $< $(CXXFLAGS) $(INC_PATHS)

build/%.o: $(BASE)/%.c
	$(info ==> Compiling file: $(notdir $<))
	@$(MK) -p $(dir $@)
	@$(CC) -MP -MMD -c -o $@ $< $(CFLAGS) $(INC_PATHS)

build/%.o: $(BASE)/%.s
	$(info ==> Assembling file: $(notdir $<))
	@$(MK) -p $(dir $@)
	@$(CC) -x assembler-with-cpp -MP -MMD -c -o $@ $< $(ASMFLAGS) $(INC_PATHS)


# Link rule.

build/$(TARGET).out: $(OBJS) build/libmm.a
	$(info ==> Linking target: $@)
	@echo $^ $(LIB_FILES) > $(@:.out=.in)
	@$(CC) $(LDFLAGS) @$(@:.out=.in) -Wl,-Map=$(@:.out=.map) -o $@ $(LDLIBS)
	@$(SIZE) $@


# Output conversion rules.

%.bin: %.out
	$(info ==> Preparing: $(notdir $@))
	@$(OBJCOPY) -O binary $< $@

%.hex: %.out
	$(info ==> Preparing: $(notdir $@))
	@$(OBJCOPY) -O ihex $< $@
