include $(SID_DIR)/buildfiles/arch/_common.mk

TOOLCHAIN_PREFIX ?= $(COMPILER_PREFIX) /opt/cs09q2/bin/mips-linux-gnu-
CXX=$(TOOLCHAIN_PREFIX)g++
CC=$(TOOLCHAIN_PREFIX)gcc

CFLAGS+=-msoft-float
LDFLAGS+=-msoft-float

CFLAGS+= $(ARCH_FLAGS)

CRYPTO_OPT_CFLAGS ?= -O3

ifeq ($(DEBUG),)
CFLAGS+= -Os
else
CFLAGS += -O0 
endif

