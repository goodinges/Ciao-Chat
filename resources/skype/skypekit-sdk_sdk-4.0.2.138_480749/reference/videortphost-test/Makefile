SID_DIR=../..
include $(SID_DIR)/buildfiles/makefile.header.mk

CONFIG = debug-$(TARGET_ARCH)

ifneq ($(MINIMAL_MODE),0)
MINIMAL_MODE_OPT = --minimal-mode
endif

all: videohost install

videohost: premake
	make -C build/gmake videortphost-test config=$(CONFIG)

loopback: premake
	make -C build/gmake loopback config=$(CONFIG)

premake:
	$(PREMAKE) --target-os=$(TARGET_OS) --target-arch=$(TARGET_ARCH) $(MINIMAL_MODE_OPT) gmake

install:
	$(PREMAKE) --target-os=$(TARGET_OS) --target-arch=$(TARGET_ARCH) install

clean:
	$(RM) -rf build

