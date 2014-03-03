SID_DIR=../..
include $(SID_DIR)/buildfiles/makefile.header.mk

CONFIG = debug-$(TARGET_ARCH)

all: voicepcmhost-file install

voicepcmhost-file: premake
ifeq ($(findstring CYGWIN_NT,$(SYSTEM)),CYGWIN_NT)
	$(MSBUILD) build/vs2005/voicepcmhost-file.vcproj /p:configuration=Release-x86
else
	make -C build/gmake $@ config=$(CONFIG) verbose=yes
endif


premake:
	$(PREMAKE) --target-os=$(TARGET_OS) --target-arch=$(TARGET_ARCH) gmake
	$(PREMAKE) --target-os=$(TARGET_OS) --target-arch=$(TARGET_ARCH) vs2005
	$(PREMAKE) --target-os=$(TARGET_OS) --target-arch=$(TARGET_ARCH) vs2008


install:
	$(PREMAKE) --target-os=$(TARGET_OS) --target-arch=$(TARGET_ARCH) install

clean:
	$(RM) -rf build

