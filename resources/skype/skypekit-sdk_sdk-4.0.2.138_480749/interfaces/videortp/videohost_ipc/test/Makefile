SID_DIR = ../../../..
TARGET_ARCH ?= x86
TARGET_OS ?= linux

ifneq ($(TARGET_ARCH),)
include $(SID_DIR)/buildfiles/arch/linux-$(TARGET_ARCH).mk
export CC CXX CFLAGS LDFLAGS 
endif

CONFIG = debug-$(TARGET_ARCH)

all: videohost

videohost: premake
	make -C build/gmake videortphost-ipctest config=$(CONFIG)

premake:
	premake4 --target-arch=$(TARGET_ARCH) $(MINIMAL_MODE_OPT) gmake

install:
	premake4 --target-os=$(TARGET_OS) --target-arch=$(TARGET_ARCH) install

clean:
	$(RM) -rf build

