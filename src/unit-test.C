#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string>
#include <error.h>
#include <argp.h>
#include <thread>
#include <atomic>
#include <time.h>


#include <gps.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <X11/Xlib.h>
#include <X11/extensions/dpms.h>



int main(int argc, char *argv[]) {
  SDL_Init(SDL_INIT_VIDEO);
  IMG_Init(IMG_INIT_PNG|IMG_INIT_TIF);
  TTF_Init();

  SDL_Window *w = SDL_CreateWindow("Unit Test", 0, 0, 1200, 720, SDL_WINDOW_SHOWN);
  SDL_Renderer *r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED);

  
  TTF_Font *f = TTF_OpenFont("/usr/share/fonts/TTF/DejaVuSansMono-Bold.ttf", 64);
  SDL_Surface *in = IMG_Load("gen/images/1200x720/doublenight.tiff");
  SDL_Texture *tn = SDL_CreateTextureFromSurface(r, in);
  SDL_FreeSurface(in);
  SDL_Surface *id = IMG_Load("gen/images/1200x720/doubleday_09.tiff");
  SDL_Texture *td = SDL_CreateTextureFromSurface(r, id);
  SDL_FreeSurface(id);


  // SDL_Texture *tt = SDL_CreateTexture(r, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1200, 720);
  // SDL_SetRenderTarget(r, tt);

  bool quit = false;
  size_t count = 0;
  uint32_t start_time = SDL_GetTicks();
  uint32_t ticks_per_frame = (uint32_t)(1000.0/60.0);
  SDL_Texture *t = td;
  while(!quit) {
    uint32_t render_start = SDL_GetTicks();
    SDL_Event e;
    while(SDL_PollEvent(&e) != 0) {
      if(e.type == SDL_QUIT) {
	printf("Event SDL_QUIT\n");
	quit = true;
      } else if(e.type == SDL_KEYDOWN) {
	printf("Event SDL_KEYDOWN (%d)\n", e.key.keysym.sym);
	if(t == td) {
	  t = tn;
	} else {
	  t = td;
	}
      } else if(e.type == SDL_MOUSEBUTTONDOWN) {
	printf("Event SDL_MOUSEBUTTONDOWN (%d)@(%d,%d)\n", e.button.button, e.button.x, e.button.y);
      } else if(e.type == SDL_FINGERDOWN) {
	printf("Event SDL_FINGERDOWN (%ld)@(%f,%f)\n", e.tfinger.touchId, e.tfinger.x, e.tfinger.y);
      }
    }

    std::string fps = std::to_string(1000.0 * count / (render_start - start_time));
    
    SDL_Surface *s = TTF_RenderText_Blended(f, fps.c_str(), {0x3f,0xff,0x3f,0});
    SDL_Texture *tt = SDL_CreateTextureFromSurface(r, s);
    
    SDL_SetRenderDrawColor(r, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(r);

    SDL_Rect st = {(int)count % 1200,0,1200,720};
    SDL_Rect stt = {50,720 - s->h - 50,s->w, s->h};
    SDL_RenderCopy(r, t, &st, NULL);
    SDL_RenderCopy(r, tt, NULL, &stt);
    SDL_RenderPresent(r);
    ++count;

    SDL_FreeSurface(s);
    SDL_DestroyTexture(tt);
    
    uint32_t render_end = SDL_GetTicks();
    if(render_end - render_start <= ticks_per_frame) {
      SDL_Delay(render_start + ticks_per_frame - render_end);
    }
  }
  uint32_t end_time = SDL_GetTicks();
  
  TTF_CloseFont(f);
  
  SDL_DestroyTexture(td);
  SDL_DestroyTexture(tn);
  SDL_DestroyRenderer(r);
  SDL_DestroyWindow(w);

  TTF_Quit();
  IMG_Quit();
  SDL_Quit();

  printf("Generated %zu frames, average fps: %f\n", count, (double)count*1000/(double)(end_time - start_time));


  
  // Test creation of a window with SDL
  /// @TODO
  
  // Test putting image into SDL window
  /// @TODO
  
  // Test putting text into the SDL window
  /// @TODO

  // Test touch-screen and mouse input events
  /// @TODO
  
  // Test keyboard events
  /// @TODO

  // Test DPMS control status, off, on
  /// @TODO
  
  // Test screen brightness control
  // My current screen on the Pi doesn't support software
  // brightness control, sadly.
  
  // Test GPS connection
  /// @TODO

  
  exit(0);
}
