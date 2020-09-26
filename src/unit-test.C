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

#define xstr(s) str(s)
#define str(s) #s

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 720
#define FONT_SIZE 48
#define TARGET_FPS 60.0

// Earth rotations per second
#define SCROLL_RATE (1.0/(60.0))

int main(int argc, char *argv[]) {
  SDL_Init(SDL_INIT_VIDEO);
  IMG_Init(IMG_INIT_PNG|IMG_INIT_TIF);
  TTF_Init();

  SDL_Window *w = SDL_CreateWindow("Unit Test", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  SDL_Renderer *r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED);

  
  TTF_Font *f = TTF_OpenFont("/usr/share/fonts/TTF/DejaVuSansMono-Bold.ttf", FONT_SIZE);
  SDL_Surface *in = IMG_Load("gen/images/" xstr(SCREEN_WIDTH) "x" xstr(SCREEN_HEIGHT) "/doublenight.tiff");
  SDL_Texture *tn = SDL_CreateTextureFromSurface(r, in);
  SDL_FreeSurface(in);
  SDL_Surface *id = IMG_Load("gen/images/" xstr(SCREEN_WIDTH) "x" xstr(SCREEN_HEIGHT) "/doubleday_09.tiff");
  SDL_Texture *td = SDL_CreateTextureFromSurface(r, id);
  SDL_FreeSurface(id);


  // SDL_Texture *tt = SDL_CreateTexture(r, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
  // SDL_SetRenderTarget(r, tt);

  bool quit = false;
  size_t count = 0;
  uint32_t start_time = SDL_GetTicks();
  double target_fps = TARGET_FPS;
  SDL_Texture *t = td;
  uint32_t last_start = 0;
  uint32_t last_end = 0;
  uint32_t total_consumed = 0;
  double offset = 0.0;
  double cur_speed = SCROLL_RATE;
  while(!quit) {
    uint32_t render_start = SDL_GetTicks();
    uint32_t ticks_per_frame = (uint32_t)(1000.0/target_fps);
    SDL_Event e;
    while(SDL_PollEvent(&e) != 0) {
      if(e.type == SDL_QUIT) {
	// printf("Event SDL_QUIT\n");
	quit = true;
      } else if(e.type == SDL_KEYDOWN) {
	//printf("Event SDL_KEYDOWN (%d)\n", e.key.keysym.sym);
	if(e.key.keysym.sym == SDLK_SPACE) {
	  if(t == td) {
	    t = tn;
	  } else {
	    t = td;
	  }
	} else if(e.key.keysym.sym == SDLK_q) {
	  quit = true;
	} else if(e.key.keysym.sym == SDLK_COMMA) {
	  cur_speed /= 1.1;
	} else if(e.key.keysym.sym == SDLK_PERIOD) {
	  cur_speed *= 1.1;
	} else if(e.key.keysym.sym == SDLK_EQUALS) {
	  target_fps += 1;
	} else if(e.key.keysym.sym == SDLK_MINUS) {
	  target_fps -= 1;
	}
      } else if(e.type == SDL_MOUSEBUTTONDOWN) {
	// printf("Event SDL_MOUSEBUTTONDOWN (%d)@(%d,%d)\n", e.button.button, e.button.x, e.button.y);
      } else if(e.type == SDL_FINGERDOWN) {
	// printf("Event SDL_FINGERDOWN (%ld)@(%f,%f)\n", e.tfinger.touchId, e.tfinger.x, e.tfinger.y);
      }
    }

    SDL_SetRenderDrawColor(r, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(r);
    
    SDL_Rect st = {(int)offset,0,SCREEN_WIDTH,SCREEN_HEIGHT};
    SDL_RenderCopy(r, t, &st, NULL);

    if(last_start > 0) {
      char buf[128];
      uint32_t ticks_consumed = last_end - last_start;
      total_consumed += ticks_consumed;
      if(ticks_consumed == 0) {
	ticks_consumed = 1;
      }
      snprintf(buf, 128, "FPS: %8.3f MAX: %8.3f RPM: %8.3f",
	       1000.0/(render_start - last_start),
	       1000.0/(ticks_consumed),
	       cur_speed * 60.0);
      
      SDL_Surface *s = TTF_RenderText_Blended(f, buf, {0x3f,0xff,0x3f,0});
      SDL_Texture *tt = SDL_CreateTextureFromSurface(r, s);
    
      SDL_Rect stt = {50,SCREEN_HEIGHT - s->h - 50,s->w, s->h};
      SDL_RenderCopy(r, tt, NULL, &stt);

      SDL_FreeSurface(s);
      SDL_DestroyTexture(tt);

      double delta_offset = (double)(render_start - last_start)/1000.0 * cur_speed * (double)SCREEN_WIDTH;
      offset += delta_offset;
      while(offset > (double)SCREEN_WIDTH) {
	offset -= (double)SCREEN_WIDTH;
      }
      //      printf("Cur_speed = %9.3f, dt %u, do %9.3f, Offset = %9.3f\n", cur_speed, render_start-last_start, delta_offset, offset);
    }
    
    SDL_RenderPresent(r);

    ++count;
    uint32_t render_end = SDL_GetTicks();

    if(render_end - render_start <= ticks_per_frame) {
      SDL_Delay(render_start + ticks_per_frame - render_end);
    }

    last_start = render_start;
    last_end = render_end;
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

  printf("Generated %zu frames, average fps: %f, max fps: %f\n", count, (double)count*1000/(double)(end_time - start_time), 1000.0*count/total_consumed);


  // Test DPMS control status, off, on
  /// @TODO
  
  // Test screen brightness control
  // My current screen on the Pi doesn't support software
  // brightness control, sadly.
  
  // Test GPS connection
  /// @TODO

  
  exit(0);
}
