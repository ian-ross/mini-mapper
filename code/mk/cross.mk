COMMON_FLAGS += -mcpu=cortex-m7
COMMON_FLAGS += -mthumb -mabi=aapcs
COMMON_FLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16

LINKER_SCRIPT = STM32F767ZITx_FLASH.ld

GCC_PREFIX=arm-none-eabi

CC      := $(GCC_PREFIX)-gcc
CXX     := $(GNU_PREFIX)-c++
AS      := $(GNU_PREFIX)-as
AR      := $(GNU_PREFIX)-ar
LD      := $(GNU_PREFIX)-ld
NM      := $(GNU_PREFIX)-nm
OBJDUMP := $(GNU_PREFIX)-objdump
OBJCOPY := $(GNU_PREFIX)-objcopy
SIZE    := $(GNU_PREFIX)-size
STRIP   := $(GNU_PREFIX)-strip

LDFLAGS += --specs=nosys.specs
LDFLAGS += -Lhw -T$(LINKER_SCRIPT)
