
TARGET_OS ?= linux
TARGET_ARCH ?= x86
SYSTEM = $(shell uname -s)
FORMAT = astyle --style=linux

ifeq ($(findstring CYGWIN_NT,$(SYSTEM)),CYGWIN_NT)
	TARGET_OS=windows
endif

ifeq ($(SYSTEM),Darwin)
	TARGET_OS=mac
endif	

include $(SID_DIR)/buildfiles/arch/$(TARGET_OS)-$(TARGET_ARCH).mk

PREMAKE ?= premake4

export CC CXX CFLAGS LDFLAGS LINKER LD AR

