SID_DIR=../../../..
include $(SID_DIR)/buildfiles/makefile.header.mk

CONFIG = debug-$(TARGET_ARCH)

all:

common-shm-test: premake
	make -C build/gmake $@ config=$(CONFIG)

premake:
	$(PREMAKE) --target-os=$(TARGET_OS) --target-arch=$(TARGET_ARCH) gmake

install:
	$(PREMAKE) --target-os=$(TARGET_OS) --target-arch=$(TARGET_ARCH) install

clean:
	$(RM) -rf build

check: common-shm-test
	./build/common-shm-test

