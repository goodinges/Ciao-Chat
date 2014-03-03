ifneq ($(TEST_ALL),)
REF_ENGINE_NAMES:=$(shell ls |egrep "(video|voice)" )
endif

.PHONY: $(REF_ENGINE_NAMES)

ifeq ($(REF_ENGINE_NAMES),)
HELP_TEXT = "Usage:\n"
HELP_TEXT += "	Build list of engines: make REF_ENGINE_NAMES=\"list of names\" TARGET_ARCH=architecture\n" 
HELP_TEXT += "	Install list of engines: make install REF_ENGINE_NAMES=\"list of names\" TARGET_ARCH=architecture\n" 
HELP_TEXT += "	Clean list of engines: make clean REF_ENGINE_NAMES=\"list of names\" TARGET_ARCH=architecture\n" 
HELP_TEXT += "	The names are the same as corresponding directory names\n" 
else
HELP_TEXT=
endif

all: $(REF_ENGINE_NAMES)
	@echo $(HELP_TEXT)

$(REF_ENGINE_NAMES):
	$(MAKE) -C $@

REF_ENGINE_NAMES_INSTALL = $(addsuffix _install,$(REF_ENGINE_NAMES))

install: $(REF_ENGINE_NAMES_INSTALL)

$(REF_ENGINE_NAMES_INSTALL):
	$(MAKE) -C $(subst _install,,$@) install

REF_ENGINE_NAMES_CLEAN = $(addsuffix _clean,$(REF_ENGINE_NAMES))

clean: $(REF_ENGINE_NAMES_CLEAN)

$(REF_ENGINE_NAMES_CLEAN):
	$(MAKE) -C $(subst _clean,,$@) clean
	

	

	