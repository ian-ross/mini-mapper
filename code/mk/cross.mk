# Build definitions for application cross-compilation builds.

# Toolchain definitions.
GCC_PREFIX := arm-none-eabi
CC      := $(GCC_PREFIX)-gcc
CXX     := $(GCC_PREFIX)-c++
AS      := $(GCC_PREFIX)-as
AR      := $(GCC_PREFIX)-gcc-ar
LD      := $(GCC_PREFIX)-ld
NM      := $(GCC_PREFIX)-nm
OBJDUMP := $(GCC_PREFIX)-objdump
OBJCOPY := $(GCC_PREFIX)-objcopy
SIZE    := $(GCC_PREFIX)-size
STRIP   := $(GCC_PREFIX)-strip

# Common ARM platform compilation flags.
COMMON_FLAGS += -mcpu=cortex-m7
COMMON_FLAGS += -mthumb -mabi=aapcs
COMMON_FLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16

# Include path for STM32 and ARM Cortex-M7 CMSIS definitions.
INCS += $(BASE)/hw/peripherals

# Linker script for STM32F767ZIx on Nucleo board.
LINKER_SCRIPT = STM32F767ZITx_FLASH.ld

# No exceptions or RTTI on STM32 platform.
CXXFLAGS += -fno-exceptions -fno-rtti

# Link flags: pick up library from build directory, linker script from
# hw directory, minimal library support.
LDFLAGS += -Lbuild -L$(BASE)/hw -T$(LINKER_SCRIPT)
LDFLAGS += -specs=nosys.specs
LDFLAGS += -Wl,--gc-sections

# Link libraries: application library plus minimal C++ support.
LDLIBS = -lmm -lc -lm -lnosys -lsupc++
