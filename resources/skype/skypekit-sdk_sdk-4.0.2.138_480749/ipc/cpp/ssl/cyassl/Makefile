SID_DIR ?= ../../../../
GMAKE_CONFIG ?= release

include $(SID_DIR)/buildfiles/makefile.header.mk

ifeq ($(findstring CYGWIN_NT,$(SYSTEM)),CYGWIN_NT)
all: vs
clean: cyassl_clean_vs
else
all: gmake
clean: cyassl_clean_gmake
endif

cyassl_clean_vs: 
	$(MSBUILD) $(SID_DIR)/ipc/cpp/ssl/cyassl/build/vs2005/skypekit-cyassl_lib.sln /t:clean /p:configuration=ReleaseStatic  
	$(MSBUILD) $(SID_DIR)/ipc/cpp/ssl/cyassl/build/vs2005/skypekit-cyassl_lib.sln /t:clean /p:configuration=ReleaseShared
	
cyassl_clean_gmake:
	$(MAKE) -C $(SID_DIR)/ipc/cpp/ssl/cyassl/build/gmake config=$(GMAKE_CONFIG)static clean
	$(MAKE) -C $(SID_DIR)/ipc/cpp/ssl/cyassl/build/gmake config=$(GMAKE_CONFIG)shared clean	

gmake:
	$(PREMAKE) --file=$(SID_DIR)/ipc/cpp/ssl/cyassl/premake4.lua --target-os=$(TARGET_OS) --target-arch=$(TARGET_ARCH) gmake
	$(MAKE) -C $(SID_DIR)/ipc/cpp/ssl/cyassl/build/gmake config=$(GMAKE_CONFIG)static verbose=1
	$(MAKE) -C $(SID_DIR)/ipc/cpp/ssl/cyassl/build/gmake config=$(GMAKE_CONFIG)shared verbose=1
	
vs:
	$(PREMAKE) --file=$(SID_DIR)/ipc/cpp/ssl/cyassl/premake4.lua --target-os=$(TARGET_OS) --target-arch=$(TARGET_ARCH) vs2005
	$(MSBUILD) $(SID_DIR)/ipc/cpp/ssl/cyassl/build/vs2005/skypekit-cyassl_lib.sln /p:configuration=ReleaseStatic
	$(MSBUILD) $(SID_DIR)/ipc/cpp/ssl/cyassl/build/vs2005/skypekit-cyassl_lib.sln /p:configuration=ReleaseShared
