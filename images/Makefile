.PHONY: all clean day night clean-source clean-gen mostlyclean distclean distclean-gen

ifndef ROOTDIR
ROOTDIR := $(CURDIR)/..
export ROOTDIR
endif

include $(ROOTDIR)/mk/variables.mk

SRCDIR := $(CURDIR)
OBJDIR := $(ROOTDIR)/gen/images/$(TARGET_GEOMETRY)

$(warning ROOTDIR=$(ROOTDIR))
$(warning CURDIR=$(CURDIR))
$(warning SRCDIR=$(SRCDIR))
$(warning OBJDIR=$(OBJDIR))
$(warning MAKECMDGOALS=$(MAKECMDGOALS))

TARGET_EXT=tiff

BASE_URL=https://eoimages.gsfc.nasa.gov/images/imagerecords

DAY_GROUP=73000
DAY_NAME=world.topo.bathy
DAY_DIM=3x5400x2700
DAY_EXT=png

NIGHT_GROUP=55000
NIGHT_NAME=land_ocean_ice_lights
NIGHT_DIM=8192
NIGHT_EXT=tif

all: day night

day: $(OBJDIR)/doubleday_01.$(TARGET_EXT) $(OBJDIR)/doubleday_02.$(TARGET_EXT) $(OBJDIR)/doubleday_03.$(TARGET_EXT) $(OBJDIR)/doubleday_04.$(TARGET_EXT) $(OBJDIR)/doubleday_05.$(TARGET_EXT) $(OBJDIR)/doubleday_06.$(TARGET_EXT) $(OBJDIR)/doubleday_07.$(TARGET_EXT) $(OBJDIR)/doubleday_08.$(TARGET_EXT) $(OBJDIR)/doubleday_09.$(TARGET_EXT) $(OBJDIR)/doubleday_10.$(TARGET_EXT) $(OBJDIR)/doubleday_11.$(TARGET_EXT) $(OBJDIR)/doubleday_12.$(TARGET_EXT)

night: $(OBJDIR)/doublenight.$(TARGET_EXT)

$(OBJDIR):
	+[ -d $(OBJDIR) ] || $(MKDIR) -p $(OBJDIR)

# Plan (for each image):
#  - download the image in question
#  - convert to standard size file, TIFF format, of given width/height
#  - double the image, side by side, to better handle viewports
# convert $< -resize $(TARGET_GEOMETRY) -duplicate 1 +append $@

$(OBJDIR)/doubleday_01.$(TARGET_EXT): $(SRCDIR)/day_73580_200401.$(DAY_EXT) | $(OBJDIR)
$(OBJDIR)/doubleday_02.$(TARGET_EXT): $(SRCDIR)/day_73605_200402.$(DAY_EXT) | $(OBJDIR)
$(OBJDIR)/doubleday_03.$(TARGET_EXT): $(SRCDIR)/day_73630_200403.$(DAY_EXT) | $(OBJDIR)
$(OBJDIR)/doubleday_04.$(TARGET_EXT): $(SRCDIR)/day_73655_200404.$(DAY_EXT) | $(OBJDIR)
$(OBJDIR)/doubleday_05.$(TARGET_EXT): $(SRCDIR)/day_73701_200405.$(DAY_EXT) | $(OBJDIR)
$(OBJDIR)/doubleday_06.$(TARGET_EXT): $(SRCDIR)/day_73726_200406.$(DAY_EXT) | $(OBJDIR)
$(OBJDIR)/doubleday_07.$(TARGET_EXT): $(SRCDIR)/day_73751_200407.$(DAY_EXT) | $(OBJDIR)
$(OBJDIR)/doubleday_08.$(TARGET_EXT): $(SRCDIR)/day_73776_200408.$(DAY_EXT) | $(OBJDIR)
$(OBJDIR)/doubleday_09.$(TARGET_EXT): $(SRCDIR)/day_73801_200409.$(DAY_EXT) | $(OBJDIR)
$(OBJDIR)/doubleday_10.$(TARGET_EXT): $(SRCDIR)/day_73826_200410.$(DAY_EXT) | $(OBJDIR)
$(OBJDIR)/doubleday_11.$(TARGET_EXT): $(SRCDIR)/day_73884_200411.$(DAY_EXT) | $(OBJDIR)
$(OBJDIR)/doubleday_12.$(TARGET_EXT): $(SRCDIR)/day_73909_200412.$(DAY_EXT) | $(OBJDIR)

$(OBJDIR)/doubleday_%.$(TARGET_EXT):
	$(CONVERT) $< -resize $(TARGET_GEOMETRY) -duplicate 1 +append $@

$(SRCDIR)/day_%.$(DAY_EXT):
	$(WGET) $(WGET_OPT) $(BASE_URL)/$(DAY_GROUP)/$(word 2, $(subst _, ,$(basename $(notdir $@))))/$(DAY_NAME).$(word 3, $(subst _, ,$(basename $(notdir $@)))).$(DAY_DIM).$(DAY_EXT) -O $@

$(OBJDIR)/doublenight.$(TARGET_EXT): $(SRCDIR)/night_55167.$(NIGHT_EXT) | $(OBJDIR)

$(OBJDIR)/doublenight.$(TARGET_EXT):
	$(CONVERT) $< -resize $(TARGET_GEOMETRY) -duplicate 1 +append $@

$(SRCDIR)/night_%.$(NIGHT_EXT):
	$(WGET) $(WGET_OPT) $(BASE_URL)/$(NIGHT_GROUP)/$(word 2, $(subst _, ,$(basename $(notdir $@))))/$(NIGHT_NAME)_$(NIGHT_DIM).$(NIGHT_EXT) -O $@

clean: clean-gen

mostlyclean: clean-distgen

distclean: clean-distgen clean-source

clean-gen:
	$(RM) -r $(OBJDIR)

clean-distgen:
	$(RM) -r $(ROOTDIR)/gen/images

clean-source:
	$(RM) -r $(SRCDIR)/day_*.$(DAY_EXT) $(SRCDIR)/night_*.$(NIGHT_EXT)
