TARGET_OS = linux
TARGET_ARCH = armv5le-eabi
VPULIB_PATH = src/lib/vpu

SID_DIR=../..
include $(SID_DIR)/buildfiles/makefile.header.mk
CONFIG = debug-$(TARGET_ARCH)

ifeq ($(MINIMAL_MODE),1)
MINIMAL_MODE_OPT = --minimal-mode
endif

all: videohost install

videohost: premake
	make -C $(VPULIB_PATH) all PLATFORM=IMX27PDK CROSS_COMPILE=$(subst g++,,$(CXX))
	make -C build/gmake videortphost-imx27 config=$(CONFIG)

loopback: premake
	make -C $(VPULIB_PATH) all PLATFORM=IMX27PDK CROSS_COMPILE=$(subst g++,,$(CXX))
	make -C build/gmake loopback config=$(CONFIG)

premake:
	$(PREMAKE) --target-os=$(TARGET_OS) --target-arch=$(TARGET_ARCH) $(MINIMAL_MODE_OPT) gmake

install:
	$(PREMAKE) --target-os=$(TARGET_OS) --target-arch=$(TARGET_ARCH) install

clean:
	$(RM) -rf build
	make -C $(VPULIB_PATH) clean
	$(RM) -f $(VPULIB_PATH)/libvpu.so $(VPULIB_PATH)/libvpu.a

