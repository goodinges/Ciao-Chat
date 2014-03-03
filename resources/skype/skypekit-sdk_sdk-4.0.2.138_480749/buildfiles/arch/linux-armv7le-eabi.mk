ARCH_FLAGS += -mlittle-endian -march=armv7-a -mtune=cortex-a8 -D__ARM_EABI_KERNEL__ 
ARCH_FLAGS += -mfpu=neon

include $(SID_DIR)/buildfiles/arch/_common-linux-arm.mk
