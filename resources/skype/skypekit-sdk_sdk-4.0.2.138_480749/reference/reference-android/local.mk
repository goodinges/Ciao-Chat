## Host system
#HOST = linux-x86
#HOST = windows
HOST = darwin-x86

## Android version
#ANDROID_VERSION = froyo
ANDROID_VERSION = gingerbread

## Android root directory
ANDROID_BASE = /opt/droid

## Android NDK root directory
ifeq ($(ANDROID_VERSION), froyo)
ANDROID_NDK_BASE := /opt/android-ndk-r4b
else
ifeq ($(ANDROID_VERSION), gingerbread)
ANDROID_NDK_BASE := /opt/android-ndk-r5c
endif
endif

## Android lib directory
ANDROID_LIBPATH = $(ANDROID_BASE)/lib

## Use Nvidia Tegra native headers and libs
NVIDIA_TANGO_NDK = false
NVIDIA_VENTANA_NDK = false

## Nvidia Tegra root directory
ifeq ($(NVIDIA_TANGO_NDK), true)
ANDROID_BASE = /opt/nv-ndk
else
ifeq ($(NVIDIA_VENTANA_NDK), true)
ANDROID_BASE = /opt/platform_gb/android
endif
endif

# SkypeKit version
#SKYPEKIT_VERSION = 3.1
#SKYPEKIT_VERSION = 3.5
SKYPEKIT_VERSION = 4.0

# SkypeKit root directory
ifeq ($(SKYPEKIT_VERSION), 3.1)
SKYPEKIT_DISTRO = /opt/distro-skypekit-cb3.1_3.1.0.2965_185661
else
SKYPEKIT_DISTRO = /opt/skypekit-beta4
endif

## SkypeKit root diretory (also change path in Java code)
SKYPEKIT_ROOT_PATH = \"/data/data/com.skype.ref/files\"
