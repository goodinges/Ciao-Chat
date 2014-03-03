NDK_PATH ?= /opt/android-ndk-r5b

TOOLCHAIN_PREFIX=ccache $(NDK_PATH)/toolchains/arm-linux-androideabi-4.4.3/prebuilt/linux-x86/bin/arm-linux-androideabi-
ARCH_FLAGS += -fpic -mthumb-interwork -ffunction-sections -funwind-tables -fstack-protector -fno-short-enums\
-Wno-psabi -march=armv7-a -msoft-float -fno-strict-aliasing -finline-limit=64  
CFLAGS += -I$(NDK_PATH)/platforms/android-8/arch-arm/usr/include
CFLAGS += -D__linux__ -DANDROID 

ifeq ($(ANDROID_APP_STL),gnustl_static)
CFLAGS += -I$(NDK_PATH)/sources/cxx-stl/gnu-libstdc++/include
CFLAGS += -I$(NDK_PATH)/sources/cxx-stl/gnu-libstdc++/libs/armeabi-v7a/include
LDFLAGS += -L$(NDK_PATH)/sources/cxx-stl/gnu-libstdc++/libs/armeabi-v7a
else ifeq ($(ANDROID_APP_STL),stlport_static)
CFLAGS += -I$(NDK_PATH)/sources/cxx-stl/stlport/stlport
LDFLAGS += -L$(NDK_PATH)/sources/cxx-stl/stlport/libs/armeabi-v7a
else ifeq ($(ANDROID_APP_STL),stlport_shared)
CFLAGS += -I$(NDK_PATH)/sources/cxx-stl/stlport/stlport
LDFLAGS += -L$(NDK_PATH)/sources/cxx-stl/stlport/libs/armeabi-v7a
endif

LDFLAGS += \
	-B$(NDK_PATH)/platforms/android-8/arch-arm/usr/lib/

include $(SID_DIR)/buildfiles/arch/_common-linux-arm.mk
