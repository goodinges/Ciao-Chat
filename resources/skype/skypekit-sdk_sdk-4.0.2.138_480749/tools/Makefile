ifneq ($(TEST_ALL),)
REF_TOOL_NAMES:=$(shell ls |grep "tool" )
endif

.PHONY: $(REF_TOOL_NAMES)

ifeq ($(REF_TOOL_NAMES),)
HELP_TEXT = "Usage:\n"
HELP_TEXT += "	Build list of engines: make REF_TOOL_NAMES=\"list of names\" TARGET_ARCH=architecture\n"
HELP_TEXT += "	Install list of engines: make install REF_TOOL_NAMES=\"list of names\" TARGET_ARCH=architecture\n"
HELP_TEXT += "	Clean list of engines: make clean REF_TOOL_NAMES=\"list of names\" TARGET_ARCH=architecture\n"
HELP_TEXT += "	The names are the same as corresponding directory names\n"
else
HELP_TEXT=
endif

all: $(REF_TOOL_NAMES)
	@echo $(HELP_TEXT)

$(REF_TOOL_NAMES):
	$(MAKE) -C $@

REF_TOOL_NAMES_INSTALL = $(addsuffix _install,$(REF_TOOL_NAMES))

install: $(REF_TOOL_NAMES_INSTALL)

$(REF_TOOL_NAMES_INSTALL):
	$(MAKE) -C $(subst _install,,$@) install

REF_TOOL_NAMES_CLEAN = $(addsuffix _clean,$(REF_TOOL_NAMES))

clean: $(REF_TOOL_NAMES_CLEAN)

$(REF_TOOL_NAMES_CLEAN):
	$(MAKE) -C $(subst _clean,,$@) clean
