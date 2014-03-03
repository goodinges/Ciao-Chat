SID_DIR=../../..
include $(SID_DIR)/buildfiles/makefile.header.mk

CONFIG = debug-$(TARGET_ARCH)

all: gstreamer-media-engine-demo

gstreamer-media-engine-demo: premake
	make -C build/gmake $@ config=$(CONFIG)

premake:
	$(PREMAKE) --target-os=$(TARGET_OS) --target-arch=$(TARGET_ARCH) gmake

install:
	$(PREMAKE) --target-os=$(TARGET_OS) --target-arch=$(TARGET_ARCH) install

clean:
	$(RM) -rf build

check: gstreamer-media-engine-demo
	LD_LIBRARY_PATH=../build/:$(LD_LIBRARY_PATH) ./build/gstreamer-media-engine-demo

