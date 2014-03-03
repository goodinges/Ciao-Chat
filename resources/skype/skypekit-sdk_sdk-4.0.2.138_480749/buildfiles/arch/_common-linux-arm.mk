include $(SID_DIR)/buildfiles/arch/_common.mk

#TOOLCHAIN_PREFIX ?= $(COMPILER_PREFIX) /opt/cs08q3/bin/arm-none-linux-gnueabi-
TOOLCHAIN_PREFIX ?= $(COMPILER_PREFIX) /opt/cs10q1/bin/arm-none-linux-gnueabi-
CXX=$(TOOLCHAIN_PREFIX)g++
CC=$(TOOLCHAIN_PREFIX)gcc

CFLAGS+=-msoft-float -D__ARMELF__ -ffunction-sections -fdata-sections -Wno-psabi 

LDFLAGS += -Wl,--gc-sections

CFLAGS+= $(ARCH_FLAGS)

CRYPTO_OPT_CFLAGS ?= -O2 -fno-schedule-insns

# Don't remove this, or -O3 may end up on compiler command line
CFLAG_MAX_OPTIMIZE = -Os

ifeq ($(DEBUG),)
CFLAGS+= -Os
else
CFLAGS += -O0 -mapcs-frame
endif

ifneq ($(ENABLE_CONTEXT_DUMPING),)
CFLAGS += -mapcs-frame
endif

ifeq ($(SAMPLING_PROFILER_SUPPORT),yes)
CFLAGS += -fno-omit-frame-pointer
ifneq ($(ENABLE_CONTEXT_DUMPING),)
# already added
else
CFLAGS += -mapcs-frame
endif
endif
