ifneq (exec,$(notdir $(patsubst %/,%,$(dir $(CURDIR)))))
include $(CURDIR)/../mk/target.mk
else
.PHONY: exec test 

include $(ROOTDIR)/mk/variables.mk

INCDIR := $(ROOTDIR)/include

$(warning ROOTDIR=$(ROOTDIR))
$(warning CURDIR=$(CURDIR))
$(warning SRCDIR=$(SRCDIR))
$(warning OBJDIR=$(OBJDIR))
$(warning INCDIR=$(INCDIR))
$(warning MAKECMDGOALS=$(MAKECMDGOALS))

VPATH = $(SRCDIR)

CXXDEFS = -DDEFAULT_IMAGE_ROOTDIR=$(DEFAULT_IMAGE_ROOTDIR) -DDEFAULT_GEOMETRY=$(DEFAULT_GEOMETRY) -DDEFAULT_LONGITUDE=$(DEFAULT_LONGITUDE) -DDEFAULT_LATITUDE=$(DEFAULT_LATITUDE) -DDEFAULT_DISPLAY_MODE=$(DEFAULT_DISPLAY_MODE) -DDEFAULT_LOCATION_MODE=$(DEFAULT_LOCATION_MODE) -DVERSION=$(VERSION) -DPKGNAME=$(PKGNAME) -DARCH=$(TARGET_ARCH_TYPE)
CXXFLAGS = -std=gnu++11 -I$(INCDIR) -Wall -Werror $(CXXDEFS) -g -rdynamic
LDLIBS = -lgps -lX11 -lXext -lSDL2 -lSDL2_image -lSDL2_ttf -lpthread -lm -lfontconfig

exec: $(PKGNAME)

test: unit-test time_test

unit-test: unit-test.o font-support.o Spacetime.o
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@

unit-test.o: unit-test.C $(INCDIR)/font-support.H

time_test: time_test.o
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@

time_test.o: time_test.C

$(PKGNAME): $(PKGNAME).o Spacetime.o Render.o PhysicalDisplay.o font-support.o
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@

$(PKGNAME).o: $(PKGNAME).C $(INCDIR)/Render.H

Render.o: Render.C $(INCDIR)/Render.H $(INCDIR)/Spacetime.H $(INCDIR)/PhysicalDisplay.H $(INCDIR)/font-support.H

PhysicalDisplay.o: PhysicalDisplay.C $(INCDIR)/PhysicalDisplay.H $(INCDIR)/Spacetime.H

Spacetime.o: Spacetime.C $(INCDIR)/Spacetime.H



endif 
