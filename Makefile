.PHONY: all clean world-clock images

all: world-clock images

world-clock: gen/world-clock

images:
	$(MAKE) -C images -j8 all

gen/world-clock: src/world-clock.C

clean:
	rm -rf gen/
	$(MAKE) -C images clean
