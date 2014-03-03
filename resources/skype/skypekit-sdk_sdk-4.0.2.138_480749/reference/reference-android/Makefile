-include local.mk

## Customize these variables in local.mk
HOST ?= linux-x86
ANDROID_VERSION ?= froyo
ANDROID_BASE ?= /opt/droid
ANDROID_NDK_BASE ?= /opt/android-ndk-r5b
ANDROID_LIBPATH ?= $(ANDROID_BASE)/lib
# NVIDIA_TANGO_NDK ?= false
# NVIDIA_VENTANA_NDK ?= false
SKYPEKIT_VERSION ?= 3.2
SKYPEKIT_DISTRO ?= /opt/skypekit-sdk_sdk-3.3.2.31_257682
SKYPEKIT_ROOT_PATH ?= \"/data/data/com.skype.ref/files\"
# 
# ifeq ($(NVIDIA_TANGO_NDK), true)
# 	ANDROID_BASE ?= /opt/nv-ndk
# 	CFLAGS_EXTRA = -DNV_OMX -DMODIFIED_STAGEFRIGHT -DSKYPEKIT_ROOT_PATH=$(SKYPEKIT_ROOT_PATH) -DFROYO
# 	ANDROID_INCLUDES = \
# 		-I$(ANDROID_BASE)/include  \
# 		-I$(ANDROID_BASE)/include/libstagefright/include \
# 		-I$(ANDROID_BASE)/include/openmax \
# 		-I$(ANDROID_BASE)/include/openmax/nv
# 	ANDROID_LIBPATH = $(ANDROID_BASE)/lib
# else
# ifeq ($(NVIDIA_VENTANA_NDK), true)
# 	ANDROID_BASE ?= /opt/platform_gb/android
# 	CFLAGS_EXTRA = -DNV_OMX -DMODIFIED_METADATA -DSKYPEKIT_ROOT_PATH=$(SKYPEKIT_ROOT_PATH) -DGINGERBREAD
# 	ANDROID_INCLUDES = \
# 		-I$(ANDROID_BASE)/include  \
# 		-I$(ANDROID_BASE)/include/libstagefright/include \
# 		-I$(ANDROID_BASE)/../tegra/include/openmax \
# 		-I$(ANDROID_BASE)/../tegra/include/openmax/il
# 	ANDROID_LIBPATH = $(ANDROID_BASE)/lib
# else
ifeq ($(ANDROID_VERSION), froyo)
	CFLAGS_EXTRA = -DSKYPEKIT_ROOT_PATH=$(SKYPEKIT_ROOT_PATH) -DFROYO
	ANDROID_INCLUDES = \
		-I$(ANDROID_BASE)/dalvik/libnativehelper/include \
		-I$(ANDROID_BASE)/external/opencore/extern_libs_v2/khronos/openmax/include \
		-I$(ANDROID_BASE)/frameworks/base/include \
		-I$(ANDROID_BASE)/frameworks/base/media/libstagefright/include \
		-I$(ANDROID_BASE)/hardware/libhardware/include \
		-I$(ANDROID_BASE)/system/core/include
else
ifeq ($(ANDROID_VERSION), gingerbread)
	CFLAGS_EXTRA = -DSKYPEKIT_ROOT_PATH=$(SKYPEKIT_ROOT_PATH) -DGINGERBREAD
	ANDROID_INCLUDES = \
		-I$(ANDROID_BASE)/dalvik/libnativehelper/include \
		-I$(ANDROID_BASE)/frameworks/base/include \
		-I$(ANDROID_BASE)/frameworks/base/include/media/stagefright/openmax \
		-I$(ANDROID_BASE)/frameworks/base/media/libstagefright/include \
		-I$(ANDROID_BASE)/hardware/libhardware/include \
		-I$(ANDROID_BASE)/system/core/include
endif
endif
# endif
# endif

ifeq ($(SKYPEKIT_VERSION), 3.1)
	CFLAGS_EXTRA += -DSKYPEKIT31
endif

## No user serviceable parts below
ifeq ($(ANDROID_VERSION), froyo)
ANDROID = android-8
else
ifeq ($(ANDROID_VERSION), gingerbread)
ANDROID = android-9
endif
endif

# Helper vars to pull SkypeKit sources
SE1=$(SKYPEKIT_DISTRO)/interfaces/skype/cpp_embedded/src
SE2=$(SKYPEKIT_DISTRO)/ipc/cpp
SE3=$(SKYPEKIT_DISTRO)/interfaces/videortp
SE4=$(SKYPEKIT_DISTRO)/reference/common
SE5=$(SKYPEKIT_DISTRO)/interfaces/voicepcm

# The name of the native library
LIBNAME = libSkypeKitVideoDemo.so

# Find all the C++ sources in the native folder
SOURCES  = $(wildcard native/*.cpp)
SOURCES2 = \
	$(SE1)/types/skype-string.cpp \
	$(SE1)/types/skype-int-list.cpp \
	$(SE1)/types/skype-string-list.cpp \
	$(SE1)/types/skype-binary.cpp
SOURCES3 = \
	$(SE1)/platform/threading/pthread/skype-thread-pthread.cpp
SOURCES4 = \
	$(SE2)/SidApi.cpp \
	$(SE2)/SidAVDummyHelpers.cpp \
	$(SE2)/SidAVTransportWrapper.cpp \
	$(SE2)/SidAVServer.cpp \
	$(SE2)/SidField.cpp \
	$(SE2)/SidProtocolBinClient.cpp \
	$(SE2)/SidProtocolBinCommon.cpp \
	$(SE2)/SidProtocolBinServer.cpp \
	$(SE2)/SidProtocolFactory.cpp
SOURCES5 = \
	$(SE2)/AVTransport/SocketTransport.cpp \
	$(SE2)/AVTransport/UnixSocket.cpp
SOURCES6= \
	$(SE3)/videohost_ipc/sidg_videortpif_server.cpp \
	$(SE3)/videohost_ipc/sidg_msg_videortpif.cpp \
	$(SE3)/videohost_ipc/sidg_videortpif_cb_client.cpp \
	$(SE3)/videohost_ipc/sidg_msg_videortpif_cb.cpp
SOURCES7= \
	$(SE4)/video/H264RtpToAnxb.cpp \
	$(SE4)/video/H264AnxbToRtp.cpp
SOURCES8= \
	$(SE5)/sidg_pcmif_server.cpp \
	$(SE5)/sidg_msg_pcmif.cpp \
	$(SE5)/sidg_pcmif_cb_client.cpp \
	$(SE5)/sidg_msg_pcmif_cb.cpp

 vpath %.cpp native: \
	$(SE1)/types:\
	$(SE1)/platform/threading/pthread:\
	$(SE2):\
	$(SE2)/AVTransport: \
	$(SE3): \
	$(SE4): \
	$(SE4):/video/ \
	$(SE5):

# Derive the object file names from the source file names
OBJS = $(SOURCES:native/%.cpp=obj/%.o) \
	$(SOURCES2:$(SE1)/types/%.cpp=obj/%.o) \
	$(SOURCES3:$(SE1)/platform/threading/pthread/%.cpp=obj/%.o) \
	$(SOURCES4:$(SE2)/%.cpp=obj/%.o) \
	$(SOURCES5:$(SE2)/AVTransport/%.cpp=obj/%.o) \
	$(SOURCES6:$(SE3)/%.cpp=obj/%.o) \
	$(SOURCES7:$(SE4)/%.cpp=obj/%.o) \
	$(SOURCES8:$(SE5)/%.cpp=obj/%.o)

# The native library with symbols for gdb (eventually)
SYMS = obj/$(LIBNAME)

# The real native library stripped of symbols
LIB = libs/armeabi/$(LIBNAME)

# pthreads forced define for inline stuff from threads.h
CFLAGS_EXTRA += -Werror -Wall -Wno-multichar -DHAVE_PTHREADS -DLOG_MASK=0x0f -DENABLE_DEBUG \
	-I$(SE1)/platform/threading/pthread \
	-I$(SE1)/platform/threading \
	-I$(SE1)/types \
	-I$(SE2)/platform/se \
	-I$(SE2)/ \
	-I$(SE3) \
	-I$(SE4) \
	-I$(SE4)/video \
	-I$(SE5)

# Android toolchain prefix
ifeq ($(ANDROID_VERSION), froyo)
ANDROID_TOOL_PREFIX = $(ANDROID_NDK_BASE)/build/prebuilt/$(HOST)/arm-eabi-4.2.1/bin/arm-eabi-
else
ifeq ($(ANDROID_VERSION), gingerbread)
ANDROID_TOOL_PREFIX = $(ANDROID_NDK_BASE)/toolchains/arm-eabi-4.4.0/prebuilt/$(HOST)/bin/arm-eabi-
endif
endif

# Android compiler flags
ifeq ($(ANDROID_VERSION), froyo)
ANDROID_CFLAGS = -march=armv5te -mtune=xscale -msoft-float -fpic -mthumb-interwork \
	-funwind-tables -fstack-protector -fno-short-enums \
	-fno-exceptions -fno-rtti \
	-D__ARM_ARCH_5__ -D__ARM_ARCH_5T__ -D__ARM_ARCH_5E__ -D__ARM_ARCH_5TE__ -DANDROID -O0 -g -fno-omit-frame-pointer \
	$(CFLAGS_EXTRA) \
	-I$(ANDROID_NDK_BASE)/build/platforms/$(ANDROID)/arch-arm/usr/include \
	$(ANDROID_INCLUDES)
else
ifeq ($(ANDROID_VERSION), gingerbread)
ANDROID_CFLAGS = -march=armv5te -mtune=xscale -msoft-float -fpic -mthumb-interwork \
	-funwind-tables -fstack-protector -fno-short-enums \
	-fno-exceptions -fno-rtti \
	-D__ARM_ARCH_5__ -D__ARM_ARCH_5T__ -D__ARM_ARCH_5E__ -D__ARM_ARCH_5TE__ -DANDROID -O0 -g -fno-omit-frame-pointer \
	$(CFLAGS_EXTRA) \
	-I$(ANDROID_NDK_BASE)/platforms/$(ANDROID)/arch-arm/usr/include/ \
	-I$(ANDROID_NDK_BASE)/sources/cxx-stl/gnu-libstdc++/include \
	-I$(ANDROID_NDK_BASE)/sources/cxx-stl/gnu-libstdc++/libs/armeabi/include \
	$(ANDROID_INCLUDES)
endif
endif

# Android link flags
ANDROID_LDFLAGS = -nostdlib -shared -Bsymbolic --no-undefined -L$(ANDROID_LIBPATH)

# Android lib dir and libs
ifeq ($(ANDROID_VERSION), froyo)
ANDROID_LIBS = -L$(ANDROID_NDK_BASE)/build/platforms/$(ANDROID)/arch-arm/usr/lib \
	-llog -lc -lstdc++ -lm \
	$(ANDROID_NDK_BASE)/build/prebuilt/$(HOST)/arm-eabi-4.2.1/lib/gcc/arm-eabi/4.2.1/interwork/libgcc.a \
	 -g \
	 -Bdynamic -landroid_runtime -lstagefright -lutils -lnativehelper \
	 -lcamera_client -lsurfaceflinger_client -lmedia
else
ifeq ($(ANDROID_VERSION), gingerbread)
ANDROID_LIBS = -L$(ANDROID_NDK_BASE)/platforms/$(ANDROID)/arch-arm/usr/lib \
	-llog -lc -lstdc++ -lm \
	$(ANDROID_NDK_BASE)/toolchains/arm-eabi-4.4.0/prebuilt/$(HOST)/lib/gcc/arm-eabi/4.4.0/libgcc.a \
	 -g \
	 -Bdynamic -landroid_runtime -lstagefright -lutils -lnativehelper \
	 -lcamera_client -lsurfaceflinger_client -lmedia
endif
endif

# Default CDT target is all, build the native library
all: $(LIB)

# Strip the symbols to create the runtime library
$(LIB): $(SYMS)
	@mkdir -p $(dir $@)
	#$(ANDROID_TOOL_PREFIX)strip --strip-debug $^ -o $@ #
	cp $^ $@

# Link the objects into the symbol library
$(SYMS): $(OBJS)
	@mkdir -p $(dir $@)
	$(ANDROID_TOOL_PREFIX)ld $(ANDROID_LDFLAGS) -o $(SYMS) $(OBJS) $(ANDROID_LIBS)

# Compile also generating dependencies with -MD
obj/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(ANDROID_TOOL_PREFIX)g++ $(ANDROID_CFLAGS) -MD -o $@ -c $<

# Optionally include the generated dependencies
-include $(OBJS:%.o=%.d)

# Clean by deleting all the objs and the lib
clean:
	rm -fr obj libs