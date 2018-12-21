#include <error.h>
#include <string>
#include <thread>
#include <atomic>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include <Render.H>
#include <Spacetime.H>
#include <PhysicalDisplay.H>

// Support a thread running to control re-rendering at fixed intervals
// Support:
// - handling display options
// - initializing the display window
// - determing the x/y dpi
// - loading in the fonts
// - loading in the satellite images
// - rendering the satellite images across the terminator
// - handling localization viewporting of the image
// - add crosshairs for current location
// - add sun location icon
// - add moon location icon
// - add rough timezone lines
// - add timezone times at the top
// - add UTC timestamp
// - add true local timestamp
// - add locale timezone timestamp
// - add sunset/sunrise, moonset/moonrise info
// - add location info
// - add satellite info
// - only handle updating the images/terminator/timezone lines/location crosshairs/sun location/moon location icons at a much less frquent interval
// - timestamps, location text, satellite info text can be updated every second.



int load_image(const char *fn) {
  IMG_Load(fn);
}

int load_font(const char *fn) {
  TTF_OpenFont(fn, 16);
}




