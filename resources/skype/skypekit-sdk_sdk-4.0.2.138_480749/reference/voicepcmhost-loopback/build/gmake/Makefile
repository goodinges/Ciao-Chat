# GNU Make solution makefile autogenerated by Premake
# Type "make help" for usage help

ifndef config
  config=debug-x86
endif
export config

PROJECTS := voicepcmhost-loopback

.PHONY: all clean help $(PROJECTS)

all: $(PROJECTS)

voicepcmhost-loopback: 
	@echo "==== Building voicepcmhost-loopback ($(config)) ===="
	@${MAKE} --no-print-directory -C . -f voicepcmhost-loopback.make

clean:
	@${MAKE} --no-print-directory -C . -f voicepcmhost-loopback.make clean

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "   debug-x86"
	@echo "   release-x86"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   clean"
	@echo "   voicepcmhost-loopback"
	@echo ""
	@echo "For more information, see http://industriousone.com/premake/quick-start"
