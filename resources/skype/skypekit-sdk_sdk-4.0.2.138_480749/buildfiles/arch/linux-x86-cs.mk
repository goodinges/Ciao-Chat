include $(SID_DIR)/buildfiles/arch/_common.mk

TOOLCHAIN_PREFIX ?= $(COMPILER_PREFIX)/opt/ia32-2010.09/bin/i686-pc-linux-gnu-
CXX=$(TOOLCHAIN_PREFIX)g++
CC=$(TOOLCHAIN_PREFIX)gcc

ARCH_FLAGS += -m32
CFLAGS += -m32
LDFLAGS += -m32

ifeq ($(DEBUG),)
CFLAGS+= -O3
else
CFLAGS += -O0 
endif
