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

#include "font-support.H"

#define xstr(s) str(s)
#define str(s) #s

#define FONT_SIZE 24
#define TARGET_FPS 61.0

// Earth rotations per second
#define SCROLL_RATE (1.0/(60.0))

int main(int argc, char *argv[]) {
  uint32_t sw = 0;
  uint32_t sh = 0;
  uint32_t sx = 0;
  uint32_t sy = 0;
  const char *ga = xstr(DEFAULT_GEOMETRY);
  if(argc > 2 && strncmp(argv[1], "-g", 2) == 0) {
    ga = argv[2];
  }

  if(ga) {
    int count = sscanf(ga, "%ux%u+%u+%u", &sw, &sh, &sx, &sy);
    printf("Looking at [%s] for geometry and found %d values.\n", ga, count);
  } else {
    printf("No DEFAULT_GEOMETRY set at build time?\n");
    exit(1);
  }

  printf("Going with geometry %ux%u+%u+%u\n", sw, sh, sx, sy);

  char geo_dir[64];
  snprintf(geo_dir, 64, "gen/images/%ux%u", sw, sh);
  std::string geo_dir_str(geo_dir);

  SDL_Init(SDL_INIT_VIDEO);
  IMG_Init(IMG_INIT_PNG|IMG_INIT_TIF);
  TTF_Init();

  SDL_Window *w = SDL_CreateWindow("Unit Test", sx, sy, sw, sh, SDL_WINDOW_SHOWN);
  SDL_Renderer *r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED);

  SDL_GL_SetSwapInterval(1);
  SDL_RendererInfo info;
  SDL_GetRendererInfo(r, &info);
  printf("Name: %s\n", info.name);
  printf("Flags: %u\n", info.flags);
  if(info.flags & SDL_RENDERER_SOFTWARE) printf("SDL_RENDERER_SOFTWARE\n");
  if(info.flags & SDL_RENDERER_ACCELERATED) printf("SDL_RENDERER_ACCELERATED\n");
  if(info.flags & SDL_RENDERER_PRESENTVSYNC) printf("SDL_RENDERER_PRESENTVSYNC\n");
  if(info.flags & SDL_RENDERER_TARGETTEXTURE) printf("SDL_RENDERER_TARGETTEXTURE\n");

  TTF_Font *f = TTF_OpenFont(findFont("DejaVuSansMono-Bold").c_str(), FONT_SIZE);

  SDL_Surface *in = IMG_Load((geo_dir_str + "/doublenight.tiff").c_str());
  SDL_Texture *tn = SDL_CreateTextureFromSurface(r, in);
  SDL_FreeSurface(in);

  SDL_Surface *id = IMG_Load((geo_dir_str + "/doubleday_09.tiff").c_str());
  SDL_Texture *td = SDL_CreateTextureFromSurface(r, id);
  SDL_FreeSurface(id);


  // SDL_Texture *tt = SDL_CreateTexture(r, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
  // SDL_SetRenderTarget(r, tt);

  bool quit = false;
  size_t count = 0;
  struct timespec start_time;
  clock_gettime(CLOCK_MONOTONIC, &start_time);
  double target_fps = TARGET_FPS;
  SDL_Texture *t = td;
  struct timespec last_start;
  memset(&last_start, 0, sizeof(struct timespec));
  struct timespec last_end;
  memset(&last_end, 0, sizeof(struct timespec));

  uint32_t total_consumed = 0;
  double offset = 0.0;
  double cur_speed = SCROLL_RATE;
  while(!quit) {
    struct timespec render_start;
    clock_gettime(CLOCK_MONOTONIC, &render_start);
    uint32_t ticks_per_frame = (uint32_t)(1e9/target_fps);
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
	printf("Event SDL_MOUSEBUTTONDOWN (%d)@(%d,%d)\n", e.button.button, e.button.x, e.button.y);
      } else if(e.type == SDL_MOUSEBUTTONUP) {
	printf("Event SDL_MOUSEBUTTONUP (%d)@(%d,%d)\n", e.button.button, e.button.x, e.button.y);
      } else if(e.type == SDL_FINGERDOWN) {
	// printf("Event SDL_FINGERDOWN (%ld)@(%f,%f)\n", e.tfinger.touchId, e.tfinger.x, e.tfinger.y);
      }
    }

    SDL_SetRenderDrawColor(r, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(r);
    
    SDL_Rect st = {(int)offset,0,(int)sw,(int)sh};
    SDL_RenderCopy(r, t, &st, NULL);

    if(last_start.tv_sec > 0 && last_start.tv_nsec > 0) {
      char buf[128];
      uint32_t ticks_consumed = 1e9*(last_end.tv_sec - last_start.tv_sec) + (last_end.tv_nsec - last_start.tv_nsec);
      total_consumed += ticks_consumed;
      if(ticks_consumed == 0) {
	ticks_consumed = 1;
      }
      snprintf(buf, 128, "FPS: %8.3f MAX: %8.3f RPM: %8.3f",
	       1e9/(1e9*(render_start.tv_sec - last_start.tv_sec) + (render_start.tv_nsec - last_start.tv_nsec)),
	       1e9/(ticks_consumed),
	       cur_speed * 60.0);
      
      SDL_Surface *s = TTF_RenderText_Blended(f, buf, {0x3f,0xff,0x3f,0});
      SDL_Texture *tt = SDL_CreateTextureFromSurface(r, s);
    
      SDL_Rect stt = {50,(int)sh - s->h - 50,s->w, s->h};
      SDL_RenderCopy(r, tt, NULL, &stt);

      SDL_FreeSurface(s);
      SDL_DestroyTexture(tt);

      double delta_offset = (double)(1e9*(render_start.tv_sec - last_start.tv_sec) + (render_start.tv_nsec - last_start.tv_nsec))/1e9 * cur_speed * (double)sw;
      offset += delta_offset;
      while(offset > (double)sw) {
	offset -= (double)sw;
      }
      //      printf("Cur_speed = %9.3f, dt %u, do %9.3f, Offset = %9.3f\n", cur_speed, render_start-last_start, delta_offset, offset);
    }
    
    SDL_RenderPresent(r);

    ++count;
    struct timespec render_end;
    clock_gettime(CLOCK_MONOTONIC, &render_end);

    struct timespec target_ts;
    target_ts.tv_sec = render_start.tv_sec;
    target_ts.tv_nsec = render_start.tv_nsec + ticks_per_frame;
    while(target_ts.tv_nsec >= 1e9) {
      target_ts.tv_nsec -= 1e9;
      target_ts.tv_sec += 1;
    }
    while(clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &target_ts, NULL) == EINTR);

    memcpy(&last_start, &render_start, sizeof(struct timespec));
    memcpy(&last_end, &render_end, sizeof(struct timespec));
  }
  struct timespec end_time;
  clock_gettime(CLOCK_MONOTONIC, &end_time);
  
  TTF_CloseFont(f);
  
  SDL_DestroyTexture(td);
  SDL_DestroyTexture(tn);
  SDL_DestroyRenderer(r);
  SDL_DestroyWindow(w);

  TTF_Quit();
  IMG_Quit();
  SDL_Quit();

  printf("Generated %zu frames, average fps: %f, max fps: %f\n", count, (double)count*1e9/(double)(1e9*(end_time.tv_sec - start_time.tv_sec) + (end_time.tv_nsec - start_time.tv_nsec)), 1e9*count/total_consumed);


  // Test DPMS control status, off, on
  /// @TODO
  
  // Test screen brightness control
  // My current screen on the Pi doesn't support software
  // brightness control, sadly.
  
  // Test GPS connection
  /// @TODO

  
  exit(0);
}
