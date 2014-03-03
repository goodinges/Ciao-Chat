ARCH_FLAGS += -mlittle-endian -march=armv6 -mtune=arm1136j-s -D__ARM_EABI_KERNEL__

include $(SID_DIR)/buildfiles/arch/_common-linux-arm.mk
