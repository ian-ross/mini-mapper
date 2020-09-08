MK := mkdir
RM := rm -rf

COMMON_FLAGS += -DSTM32F767xx

INCS += hw lib

INC_FLAGS := $(addprefix -I,$(INCS))

COMMON_CFLAGS = \
  -ffunction-sections -fdata-sections \
  -fno-strict-aliasing -fno-builtin -fshort-enums

CFLAGS += $(COMMON_FLAGS) $(COMMON_CFLAGS) $(INC_FLAGS)
CXXFLAGS += $(COMMON_FLAGS) $(COMMON_CFLAGS) $(INC_FLAGS)
LDFLAGS += $(COMMON_FLAGS)
