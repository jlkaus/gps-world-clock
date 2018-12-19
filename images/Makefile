.PHONY: all clean day night clean-source clean-gen

# Lenovo Yoga-3 Pro: 3200x1800 (1600x900 quad)
# 5" RPi screen: 800x480
# 7" RPi screen: 1024x600 (1920x1080 ?)
TARGET_WIDTH=3200
TARGET_HEIGHT=1800
TARGET_EXT=tiff

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

day: doubleday_01.$(TARGET_EXT) doubleday_02.$(TARGET_EXT) doubleday_03.$(TARGET_EXT) doubleday_04.$(TARGET_EXT) doubleday_05.$(TARGET_EXT) doubleday_06.$(TARGET_EXT) doubleday_07.$(TARGET_EXT) doubleday_08.$(TARGET_EXT) doubleday_09.$(TARGET_EXT) doubleday_10.$(TARGET_EXT) doubleday_11.$(TARGET_EXT) doubleday_12.$(TARGET_EXT)

night: doublenight.$(TARGET_EXT)

# Plan (for each image):
#  - download the image in question
#  - convert to standard size file, TIFF format, of given width/height
#  - double the image, side by side, to better handle viewports
# convert $< -resize $(TARGET_WIDTH)x$(TARGET_HEIGHT) -duplicate 1 +append $@

doubleday_01.$(TARGET_EXT): day_73580_200401.$(DAY_EXT)
doubleday_02.$(TARGET_EXT): day_73605_200402.$(DAY_EXT)
doubleday_03.$(TARGET_EXT): day_73630_200403.$(DAY_EXT)
doubleday_04.$(TARGET_EXT): day_73655_200404.$(DAY_EXT)
doubleday_05.$(TARGET_EXT): day_73701_200405.$(DAY_EXT)
doubleday_06.$(TARGET_EXT): day_73726_200406.$(DAY_EXT)
doubleday_07.$(TARGET_EXT): day_73751_200407.$(DAY_EXT)
doubleday_08.$(TARGET_EXT): day_73776_200408.$(DAY_EXT)
doubleday_09.$(TARGET_EXT): day_73801_200409.$(DAY_EXT)
doubleday_10.$(TARGET_EXT): day_73826_200410.$(DAY_EXT)
doubleday_11.$(TARGET_EXT): day_73884_200411.$(DAY_EXT)
doubleday_12.$(TARGET_EXT): day_73909_200412.$(DAY_EXT)

doubleday_%.$(TARGET_EXT):
	convert $< -resize $(TARGET_WIDTH)x$(TARGET_HEIGHT) -duplicate 1 +append $@

day_%.$(DAY_EXT):
	$(WGET) $(WGET_OPT) $(BASE_URL)/$(DAY_GROUP)/$(word 2, $(subst _, ,$(basename $(notdir $@))))/$(DAY_NAME).$(word 3, $(subst _, ,$(basename $(notdir $@)))).$(DAY_DIM).$(DAY_EXT) -O $@

doublenight.$(TARGET_EXT): night_55167.$(NIGHT_EXT)

doublenight.$(TARGET_EXT):
	convert $< -resize $(TARGET_WIDTH)x$(TARGET_HEIGHT) -duplicate 1 +append $@

night_%.$(NIGHT_EXT):
	$(WGET) $(WGET_OPT) $(BASE_URL)/$(NIGHT_GROUP)/$(word 2, $(subst _, ,$(basename $(notdir $@))))/$(NIGHT_NAME)_$(NIGHT_DIM).$(NIGHT_EXT) -O $@

clean: clean-gen clean-source

clean-source:
	rm -rf day_*.$(DAY_EXT) night_*.$(NIGHT_EXT)

clean-gen:
	rm -rf doubleday_*.$(TARGET_EXT) doublenight.$(TARGET_EXT)
