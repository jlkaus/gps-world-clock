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
ifndef TARGET_GEOMETRY
TARGET_GEOMETRY := $(shell $(ROOTDIR)/mk/get-geometry)
export TARGET_GEOMETRY
endif

TAR = tar
MKDIR = mkdir
INSTALL = install
WGET=wget
WGET_OPT= --quiet --compression=auto
CONVERT=convert
FIND=find
CP=cp
CHMOD=chmod

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

DEFAULT_IMAGE_ROOTDIR ?= $(datadir)/$(PKGNAME)

DEFAULT_GEOMETRY ?= $(TARGET_GEOMETRY)
# Lenovo Yoga-3 Pro (13.25"): 3200x1800 (1600x900 quad) [277 dpi] 
# 5" RPi screen: 800x480 [184 dpi]
# 7" RPi screen: 1024x600 (1920x1080 ?)
# 5" segment of lenovo screen: 1200x720
# Samsung 4K monitor (28"): 3840x2160 (1920x1080 quad) [158 dpi]
# 5" segment of samsung 4k screen: 680x400

DEFAULT_LOCATION ?= "43.761463,-90.490470"
# 43.761463,-90.490470 - The South Ridge cemetary
# 45.000000,-90.000000 - Near Wausau
# 45.035423,-87.885620 - Mom & Dad's in Grover
# 44.074288,-92.557447 - My house in Rochester
# 43.764016,-90.478865 - Pasch Farm
# 43.750741,-90.449193 - Kaus Farm



DEFAULT_DISPLAY_MODE ?= 0x00000000
# 0x0000 - default, everything enabled
# 0x0001 - disable city lights for night time (just use daylight image, tinted dark)
# 0x0002 - disable month-specific daylight images, just use one image.
# 0x0004 - disable centering on current longitude
# 0x0008 - disable current location crosshairs
# 0x0010 - disable showing current sun position
# 0x0020 - disable showing current moon position
# 0x0040 - disable showing the terminator at all
# 0x0080 - disable showing true local time
# 0x0100 - disable showing the local timezone time
# 0x0200 - disable showing the UTC time
# 0x0400 - disable showing the local sunrise/sunset times
# 0x0800 - disable showing the local moonrise/moonset times
# 0x1000 - disable showing the current location
# 0x2000 - disable showing the satellite information
# 0x4000 - disable showing the rough timezone dividers
# 0x8000 - disable showing the timezone times at the top of the image

DEFAULT_LOCATION_MODE ?= auto
# gps - use GPS location/time data (fail if not available)
# system - force using system-wide time data & the user specified (or default) location
# time-only - force using system-wide time data, and no location info at all
#             Note that this sets some DISPLAY_MODE parameters as well (0x3C8C)
# auto - use 'gps' if we can, otherwise use 'system'


$(warning PKGNAME=$(PKGNAME))
$(warning VERSION=$(VERSION))
$(warning TARGET_ARCH_TYPE=$(TARGET_ARCH_TYPE))
$(warning TARGET_GEOMETRY=$(TARGET_GEOMETRY))

$(warning DESTDIR=$(DESTDIR))
$(warning prefix=$(prefix))
