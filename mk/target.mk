.SUFFIXES:

include $(CURDIR)/../mk/variables.mk

OBJDIR := $(CURDIR)/../gen/exec/$(TARGET_ARCH_TYPE)
SRCDIR := $(CURDIR)

$(warning ROOTDIR=$(ROOTDIR))
$(warning CURDIR=$(CURDIR))
$(warning SRCDIR=$(SRCDIR))
$(warning OBJDIR=$(OBJDIR))
$(warning MAKECMDGOALS=$(MAKECMDGOALS))

MAKETARGET = $(MAKE) --no-print-directory -C $@ -f $(CURDIR)/Makefile \
			SRCDIR=$(SRCDIR) $(MAKECMDGOALS)

.PHONY: $(OBJDIR)
$(OBJDIR):
	+[ -d $@ ] || $(MKDIR) -p $@
	+@$(MAKETARGET)

Makefile : ;
%.mk :: ;

% :: $(OBJDIR) ; :

.PHONY: clean clean-gen mostlyclean distclean clean-distgen clean-source

clean: clean-gen

mostlyclean: clean-distgen

distclean: clean-distgen clean-source

clean-gen:
	$(RM) -r $(OBJDIR)/

clean-distgen:
	$(RM) -r $(CURDIR)/../gen/exec/

clean-source: ;
