ARCH_FLAGS += -mlittle-endian -march=armv5te -mtune=arm926ej-s -D__ARM_EABI_KERNEL__

include $(SID_DIR)/buildfiles/arch/_common-linux-arm.mk
