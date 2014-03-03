SID_DIR=../..
include $(SID_DIR)/buildfiles/makefile.header.mk

CONFIG = debug-$(TARGET_ARCH)


all: gstreamer-media-engine install

gstreamer-media-engine: premake
	make -C build/gmake $@ config=$(CONFIG)

premake:
	$(PREMAKE) --target-os=$(TARGET_OS) --target-arch=$(TARGET_ARCH) gmake

install:
	$(PREMAKE) --target-os=$(TARGET_OS) --target-arch=$(TARGET_ARCH) install

clean:
	$(RM) -rf build
	$(RM) -rf tests/build
	make -C docs/ clean

check: gstreamer-media-engine
	make -C tests $@ config=$(CONFIG)

docs/Makefile:
	cd docs/; ./autogen.sh

docs: gstreamer-media-engine docs/Makefile
	make -C docs/
