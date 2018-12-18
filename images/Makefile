.PHONY: all clean day night

WGET=wget
WGET_OPT= -nc -c

BASE_URL=https://eoimages.gsfc.nasa.gov/images/imagerecords


all: day night

day:
	$(WGET) $(WGET_OPT) $(BASE_URL)/73000/73630/world.topo.bathy.200403.3x5400x2700.png
	$(WGET) $(WGET_OPT) $(BASE_URL)/73000/73992/world.200403.3x5400x2700.png
	$(WGET) $(WGET_OPT) $(BASE_URL)/73000/73967/world.200402.3x5400x2700.png
	$(WGET) $(WGET_OPT) $(BASE_URL)/74000/74218/world.200412.3x5400x2700.png

night:
	$(WGET) $(WGET_OPT) $(BASE_URL)/55000/55167/land_ocean_ice_lights_2048.tif
	$(WGET) $(WGET_OPT) $(BASE_URL)/55000/55167/earth_lights_4800.tif
	$(WGET) $(WGET_OPT) $(BASE_URL)/55000/55167/land_ocean_ice_lights_8192.tif
	$(WGET) $(WGET_OPT) $(BASE_URL)/55000/55167/land_lights_16384.tif


clean:
	rm -rf *.png *.tif
