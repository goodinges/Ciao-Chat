# GNU Make project makefile autogenerated by Premake
ifndef config
  config=debug-x86
endif

ifndef verbose
  SILENT = @
endif

ifndef CC
  CC = gcc
endif

ifndef CXX
  CXX = g++
endif

ifndef AR
  AR = ar
endif

ifeq ($(config),debug-x86)
  OBJDIR     = obj/Debug-x86
  TARGETDIR  = ..
  TARGET     = $(TARGETDIR)/voicepcmhost-rtaudio
  DEFINES   += -D_DEBUG -DDEBUG -D__LINUX_ALSA__
  INCLUDES  += -I../../rtaudio-4.0.6 -I../../../../interfaces/voicepcm -I../../../../interfaces/skype/cpp_embedded/src/types -I../../../../ipc/cpp/platform/se -I../../../../interfaces/skype/cpp_embedded/src/platform/threading -I../../../../interfaces/skype/cpp_embedded/src/platform/threading/pthread -I../../../../ipc/cpp -I../../../../ipc/cpp/AVTransport
  CPPFLAGS  += -MMD -MP $(DEFINES) $(INCLUDES)
  CFLAGS    += $(CPPFLAGS) $(ARCH) -Wall -Werror -g -fno-use-cxa-atexit
  CXXFLAGS  += $(CFLAGS) -fno-rtti -fno-exceptions
  LDFLAGS   += -lpthread -lasound
  LIBS      += 
  RESFLAGS  += $(DEFINES) $(INCLUDES) 
  LDDEPS    += 
  LINKCMD    = $(CXX) -o $(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(ARCH) $(LIBS)
  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
endif

ifeq ($(config),release-x86)
  OBJDIR     = obj/Release-x86
  TARGETDIR  = ..
  TARGET     = $(TARGETDIR)/voicepcmhost-rtaudio
  DEFINES   += -DNDEBUG -D__LINUX_ALSA__
  INCLUDES  += -I../../rtaudio-4.0.6 -I../../../../interfaces/voicepcm -I../../../../interfaces/skype/cpp_embedded/src/types -I../../../../ipc/cpp/platform/se -I../../../../interfaces/skype/cpp_embedded/src/platform/threading -I../../../../interfaces/skype/cpp_embedded/src/platform/threading/pthread -I../../../../ipc/cpp -I../../../../ipc/cpp/AVTransport
  CPPFLAGS  += -MMD -MP $(DEFINES) $(INCLUDES)
  CFLAGS    += $(CPPFLAGS) $(ARCH) -Wall -Werror -O2 -fno-use-cxa-atexit
  CXXFLAGS  += $(CFLAGS) -fno-rtti -fno-exceptions
  LDFLAGS   += -s -lpthread -lasound
  LIBS      += 
  RESFLAGS  += $(DEFINES) $(INCLUDES) 
  LDDEPS    += 
  LINKCMD    = $(CXX) -o $(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(ARCH) $(LIBS)
  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
endif

OBJECTS := \
	$(OBJDIR)/RtAudioPCMInterface.o \
	$(OBJDIR)/RtAudio.o \
	$(OBJDIR)/sidg_msg_pcmif.o \
	$(OBJDIR)/SidPCMHostMain.o \
	$(OBJDIR)/sidg_pcmif_cb_client.o \
	$(OBJDIR)/sidg_pcmif_server.o \
	$(OBJDIR)/sidg_msg_pcmif_cb.o \
	$(OBJDIR)/SidApi.o \
	$(OBJDIR)/SidAVDummyHelpers.o \
	$(OBJDIR)/SidAVTransportWrapper.o \
	$(OBJDIR)/SidAVServer.o \
	$(OBJDIR)/SidField.o \
	$(OBJDIR)/SidProtocolBinClient.o \
	$(OBJDIR)/SidProtocolBinCommon.o \
	$(OBJDIR)/SidProtocolBinServer.o \
	$(OBJDIR)/SidProtocolFactory.o \
	$(OBJDIR)/SocketTransport.o \
	$(OBJDIR)/skype-string.o \
	$(OBJDIR)/skype-int-list.o \
	$(OBJDIR)/skype-string-list.o \
	$(OBJDIR)/skype-binary.o \
	$(OBJDIR)/UnixSocket.o \
	$(OBJDIR)/skype-thread-pthread.o \

RESOURCES := \

SHELLTYPE := msdos
ifeq (,$(ComSpec)$(COMSPEC))
  SHELLTYPE := posix
endif
ifeq (/bin,$(findstring /bin,$(SHELL)))
  SHELLTYPE := posix
endif

.PHONY: clean prebuild prelink

all: $(TARGETDIR) $(OBJDIR) prebuild prelink $(TARGET)
	@:

$(TARGET): $(GCH) $(OBJECTS) $(LDDEPS) $(RESOURCES)
	@echo Linking voicepcmhost-rtaudio
	$(SILENT) $(LINKCMD)
	$(POSTBUILDCMDS)

$(TARGETDIR):
	@echo Creating $(TARGETDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(TARGETDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(TARGETDIR))
endif

$(OBJDIR):
	@echo Creating $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(OBJDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(OBJDIR))
endif

clean:
	@echo Cleaning voicepcmhost-rtaudio
ifeq (posix,$(SHELLTYPE))
	$(SILENT) rm -f  $(TARGET)
	$(SILENT) rm -rf $(OBJDIR)
else
	$(SILENT) if exist $(subst /,\\,$(TARGET)) del $(subst /,\\,$(TARGET))
	$(SILENT) if exist $(subst /,\\,$(OBJDIR)) rmdir /s /q $(subst /,\\,$(OBJDIR))
endif

prebuild:
	$(PREBUILDCMDS)

prelink:
	$(PRELINKCMDS)

ifneq (,$(PCH))
$(GCH): $(PCH)
	@echo $(notdir $<)
	-$(SILENT) cp $< $(OBJDIR)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
endif

$(OBJDIR)/RtAudioPCMInterface.o: ../../RtAudioPCMInterface.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/RtAudio.o: ../../rtaudio-4.0.6/RtAudio.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/sidg_msg_pcmif.o: ../../../../interfaces/voicepcm/sidg_msg_pcmif.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/SidPCMHostMain.o: ../../../../interfaces/voicepcm/SidPCMHostMain.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/sidg_pcmif_cb_client.o: ../../../../interfaces/voicepcm/sidg_pcmif_cb_client.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/sidg_pcmif_server.o: ../../../../interfaces/voicepcm/sidg_pcmif_server.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/sidg_msg_pcmif_cb.o: ../../../../interfaces/voicepcm/sidg_msg_pcmif_cb.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/SidApi.o: ../../../../ipc/cpp/SidApi.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/SidAVDummyHelpers.o: ../../../../ipc/cpp/SidAVDummyHelpers.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/SidAVTransportWrapper.o: ../../../../ipc/cpp/SidAVTransportWrapper.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/SidAVServer.o: ../../../../ipc/cpp/SidAVServer.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/SidField.o: ../../../../ipc/cpp/SidField.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/SidProtocolBinClient.o: ../../../../ipc/cpp/SidProtocolBinClient.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/SidProtocolBinCommon.o: ../../../../ipc/cpp/SidProtocolBinCommon.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/SidProtocolBinServer.o: ../../../../ipc/cpp/SidProtocolBinServer.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/SidProtocolFactory.o: ../../../../ipc/cpp/SidProtocolFactory.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/SocketTransport.o: ../../../../ipc/cpp/AVTransport/SocketTransport.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/skype-string.o: ../../../../interfaces/skype/cpp_embedded/src/types/skype-string.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/skype-int-list.o: ../../../../interfaces/skype/cpp_embedded/src/types/skype-int-list.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/skype-string-list.o: ../../../../interfaces/skype/cpp_embedded/src/types/skype-string-list.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/skype-binary.o: ../../../../interfaces/skype/cpp_embedded/src/types/skype-binary.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/UnixSocket.o: ../../../../ipc/cpp/AVTransport/UnixSocket.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/skype-thread-pthread.o: ../../../../interfaces/skype/cpp_embedded/src/platform/threading/pthread/skype-thread-pthread.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
