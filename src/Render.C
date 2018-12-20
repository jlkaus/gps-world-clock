#include <error.h>
#include <string>
#include <thread>
#include <atomic>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include <Render.H>


int load_image(const char *fn) {
  IMG_Load(fn);
}

int load_font(const char *fn) {
  TTF_OpenFont(fn, 16);
}




