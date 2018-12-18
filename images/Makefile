.PHONY: all clean day night day-source night-source

# Lenovo Yoga-3 Pro: 3200x1800 (1600x900 quad)
# 5" RPi screen: 800x480
# 7" RPi screen: 1024x600 (1920x1080 ?)
TARGET_WIDTH=3200
TARGET_HEIGHT=1800
TARGET_EXT=tiff

# Plan (for each image):
#  - download the image in question
#  - convert to standard size file, TIFF format, of given width/height
#  - double the image, side by side, to better handle viewports




WGET=wget
WGET_OPT= --quiet --compression=auto

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

day: day-source

night: night-source


day-source: day_73580_200401.$(DAY_EXT) day_73605_200402.$(DAY_EXT) day_73630_200403.$(DAY_EXT) day_73655_200404.$(DAY_EXT) day_73701_200405.$(DAY_EXT) day_73726_200406.$(DAY_EXT) day_73751_200407.$(DAY_EXT) day_73776_200408.$(DAY_EXT) day_73801_200409.$(DAY_EXT) day_73826_200410.$(DAY_EXT) day_73884_200411.$(DAY_EXT) day_73909_200412.$(DAY_EXT)

day_%.$(DAY_EXT):
	$(WGET) $(WGET_OPT) $(BASE_URL)/$(DAY_GROUP)/$(word 2, $(subst _, ,$(basename $(notdir $@))))/$(DAY_NAME).$(word 3, $(subst _, ,$(basename $(notdir $@)))).$(DAY_DIM).$(DAY_EXT) -O $@

night-source: night_55167.$(NIGHT_EXT)

night_%.$(NIGHT_EXT):
	$(WGET) $(WGET_OPT) $(BASE_URL)/$(NIGHT_GROUP)/$(word 2, $(subst _, ,$(basename $(notdir $@))))/$(NIGHT_NAME)_$(NIGHT_DIM).$(NIGHT_EXT) -O $@

clean:
	rm -rf day_*.$(DAY_EXT) night_*.$(NIGHT_EXT)
