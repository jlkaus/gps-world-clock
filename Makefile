.PHONY: all exec images clean distclean mostlyclean package install uninstall distclean-images distclean-exec mostlyclean-images mostlyclean-exec clean-exec clean-images clean-install install-dir distclean-install mostlyclean-install

ifndef ROOTDIR
ROOTDIR := $(CURDIR)
export ROOTDIR
endif

include $(ROOTDIR)/mk/variables.mk

INSTALLDIR := $(ROOTDIR)/gen/install

all: images exec

exec:
	$(MAKE) -C $(ROOTDIR)/src all

images:
	$(MAKE) -C $(ROOTDIR)/images -j8 all

install-dir: clean-install
	$(MKDIR) -p $(INSTALLDIR)
	$(MAKE) -C $(CURDIR) DESTDIR=$(INSTALLDIR) install

package: install-dir
	$(TAR) cf $(ROOTDIR)/gen/$(PKGNAME)-$(VERSION)-$(TARGET_ARCH_TYPE).tar.gz -C $(INSTALLDIR) .

install: images exec
	$(INSTALL) -D -m 755 -t $(DESTDIR)$(bindir)/$(PKGNAME) $(ROOTDIR)/gen/exec/$(TARGET_ARCH_TYPE)/$(PKGNAME)
	$(INSTALL) -D -m 644 -t $(DESTDIR)$(datadir)/$(PKGNAME) $(ROOTDIR)/gen/images/$(TARGET_GEOMETRY)/*

uninstall:
	$(RM) $(DESTDIR)$(bindir)/$(PKGNAME)
	$(RM) -r $(DESTDIR)$(datadir)/$(PKGNAME)

clean: clean-exec clean-images clean-install

distclean: distclean-exec distclean-images distclean-install
	$(RM) -r $(ROOTDIR)/gen

mostlyclean: mostlyclean-exec mostlyclean-images mostlyclean-install

clean-exec:
	$(MAKE) -C $(ROOTDIR)/src clean

mostlyclean-exec:
	$(MAKE) -C $(ROOTDIR)/src mostlyclean

distclean-exec:
	$(MAKE) -C $(ROOTDIR)/src distclean

clean-images:
	$(MAKE) -C $(ROOTDIR)/images clean

mostlyclean-images:
	$(MAKE) -C $(ROOTDIR)/images mostlyclean

distclean-images:
	$(MAKE) -C $(ROOTDIR)/images distclean

clean-install: distclean-install

mostlyclean-install: distclean-install

distclean-install:
	$(RM) -r $(INSTALLDIR)
