SID_ARCH_INCLUDED := yes

ifeq ($(INTERNAL_BUILD),yes)
CFLAGS+= -g
endif

STLPORT_PATH ?= $(SID_DIR)/ext_libs/stlport/stlport
export STLPORT_PATH 
