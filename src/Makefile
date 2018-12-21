ifneq (exec,$(notdir $(patsubst %/,%,$(dir $(CURDIR)))))
include $(CURDIR)/../mk/target.mk
else
include $(ROOTDIR)/mk/variables.mk

INCDIR := $(ROOTDIR)/include

$(warning ROOTDIR=$(ROOTDIR))
$(warning CURDIR=$(CURDIR))
$(warning SRCDIR=$(SRCDIR))
$(warning OBJDIR=$(OBJDIR))
$(warning INCDIR=$(INCDIR))
$(warning MAKECMDGOALS=$(MAKECMDGOALS))

VPATH = $(SRCDIR)

CXXFLAGS = -std=gnu++11 -I$(INCDIR) -DDEFAULT_IMAGE_ROOTDIR=$(DEFAULT_IMAGE_ROOTDIR) -DDEFAULT_GEOMETRY=$(DEFAULT_GEOMETRY) -DDEFAULT_LOCATION=$(DEFAULT_LOCATION) -DDEFAULT_DISPLAY_MODE=$(DEFAULT_DISPLAY_MODE) -DDEFAULT_LOCATION_MODE=$(DEFAULT_LOCATION_MODE) -DVERSION=$(VERSION) -DPKGNAME=$(PKGNAME) -DARCH=$(TARGET_ARCH_TYPE)
LDLIBS = -lgps -lX11 -lXext -lSDL -lSDL_image -lSDL_ttf -lpthread -lm

all: $(PKGNAME)

$(PKGNAME): $(PKGNAME).o Spacetime.o Render.o PhysicalDisplay.o

$(PKGNAME).o: $(PKGNAME).C $(INCDIR)/Render.H

Render.o: Render.C $(INCDIR)/Render.H $(INCDIR)/Spacetime.H $(INCDIR)/PhysicalDisplay.H

Spacetime.o: Spacetime.C $(INCDIR)/Spacetime.H

PhysicalDisplay.o: PhysicalDisplay.C $(INCDIR)/PhysicalDisplay.H


endif 
