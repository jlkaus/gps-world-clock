PKGNAME = gps-world-clock

ifndef ROOTDIR
ROOTDIR := $(CURDIR)
export ROOTDIR
endif

ifndef TARGET_ARCH_TYPE
TARGET_ARCH_TYPE := $(shell $(ROOTDIR)/mk/get-arch)
export TARGET_ARCH_TYPE
endif
ifndef VERSION
VERSION := $(shell cat $(ROOTDIR)/VERSION)
export VERSION
endif
ifndef TARGET_GEOMTRY
TARGET_GEOMETRY := $(shell $(ROOTDIR)/mk/get-geometry)
export TARGET_GEOMETRY
endif


# Lenovo Yoga-3 Pro (13.25"): 3200x1800 (1600x900 quad) [277 dpi] 
# 5" RPi screen: 800x480 [184 dpi]
# 7" RPi screen: 1024x600 (1920x1080 ?)
# 5" segment of lenovo screen: 1200x720


TAR = tar
MKDIR = mkdir
INSTALL = install
WGET=wget
WGET_OPT= --quiet --compression=auto
CONVERT=convert


DESTDIR =
prefix = /usr/local
exec_prefix = $(prefix)
bindir = $(exec_prefix)/bin
sbindir = $(exec_prefix)/sbin
libexecdir = $(exec_prefix)/libexec
datarootdir = $(prefix)/share
datadir = $(datarootdir)
sysconfdir = $(prefix)/etc
sharedstatedir = $(prefix)/com
localstatedir = $(prefix)/var
runstatedir = $(localstatedir)/run
includedir = $(prefix)/include
docdir = $(datarootdir)/doc/$(PKGNAME)
infodir = $(datarootdir)/info
libdir = $(exec_prefix)/lib
mandir = $(datarootdir)/man

$(warning PKGNAME=$(PKGNAME))
$(warning VERSION=$(VERSION))
$(warning TARGET_ARCH_TYPE=$(TARGET_ARCH_TYPE))
$(warning TARGET_GEOMETRY=$(TARGET_GEOMETRY))

$(warning DESTDIR=$(DESTDIR))
$(warning prefix=$(prefix))
