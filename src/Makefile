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

CXXFLAGS = -std=gnu++11 -I$(INCDIR)
LDLIBS = -lgps -lX11 -lXext -lSDL -lSDL_image -lSDL_ttf -lpthread -lm

all: $(PKGNAME)

$(PKGNAME): $(PKGNAME).o Spacetime.o Render.o PhysicalDisplay.o

$(PKGNAME).o: $(PKGNAME).C $(INCDIR)/Spacetime.H $(INCDIR)/Render.H $(INCDIR)/PhysicalDisplay.H

Spacetime.o: Spacetime.C $(INCDIR)/Spacetime.H

Render.o: Render.C $(INCDIR)/Render.H

PhysicalDisplay.o: PhysicalDisplay.C $(INCDIR)/PhysicalDisplay.H


endif 
