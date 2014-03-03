SID_DIR=../../..
include $(SID_DIR)/buildfiles/makefile.header.mk

CONFIG = debug-$(TARGET_ARCH)

all: media-engine-test

media-engine-test: premake
	make -C build/gmake $@ config=$(CONFIG)

premake:
	$(PREMAKE) --target-os=$(TARGET_OS) --target-arch=$(TARGET_ARCH) gmake

install:
	$(PREMAKE) --target-os=$(TARGET_OS) --target-arch=$(TARGET_ARCH) install

clean:
	$(RM) -rf build

check: media-engine-test
	LD_LIBRARY_PATH=../build/:$(LD_LIBRARY_PATH) MEDIA_ENGINE_PREFERENCE_DIR=../ ./build/media-engine-test

