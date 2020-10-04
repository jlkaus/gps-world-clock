#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string>
#include <error.h>
#include <argp.h>
#include <thread>
#include <atomic>
#include <time.h>
#include <unistd.h>
#include <execinfo.h>
#include <signal.h>

#include <gps.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <X11/Xlib.h>
#include <X11/extensions/dpms.h>

#include "font-support.H"
#include "Spacetime.H"

#define xstr(s) str(s)
#define str(s) #s

#define TARGET_FPS 61.0

// Earth rotations per second
#define SCROLL_RATE (1.0/(60.0))



void handler(int sig) {
  void *array[10];
  size_t size;

  // get void*'s for all entries on the stack
  size = backtrace(array, 10);

  // print out all the frames to stderr
  fprintf(stderr, "Error: signal %d:\n", sig);
  backtrace_symbols_fd(array, size, STDERR_FILENO);
  exit(1);
}

int main(int argc, char *argv[]) {
  signal(SIGSEGV, handler);
  
  Spacetime::init();
  SDuration tai_offset = Spacetime::getCurrentTAIOffset();
  SDuration tz_offset = Spacetime::getLocalTimezoneOffset();
  SMoment now_mono = SMoment::nowMono();
  SMoment now_utc_sys = SMoment::nowSystem_UTC();
  SMoment now_utc_gps = SMoment::nowGPS_UTC();

  printf("TAI_Offset: %ld\n", tai_offset.s);
  printf("TZ_Offset:  %ld\n", tz_offset.s);
  printf("Now mono:      %ld.%09ld\n", now_mono.s, now_mono.ns);
  printf("    UTC(sys):  %ld.%09ld\n", now_utc_sys.s, now_utc_sys.ns);
  printf("    UTC(gps):  %ld.%09ld\n", now_utc_gps.s, now_utc_gps.ns);
  
  
  int fontsize = 24;
  uint32_t sw = 0;
  uint32_t sh = 0;
  uint32_t sx = 0;
  uint32_t sy = 0;

  Display *xd = XOpenDisplay(NULL);
  int xsi = XDefaultScreen(xd);
  sw = XDisplayWidth(xd, xsi);
  sh = XDisplayHeight(xd, xsi);
  int swmm = XDisplayWidthMM(xd, xsi);
  int shmm = XDisplayHeightMM(xd, xsi);

  printf("Default screen found %ux%u (%d mm x %d mm)\n", sw, sh, swmm, shmm);
  
  const char *ga = NULL; //xstr(DEFAULT_GEOMETRY);
  if(argc > 2 && strncmp(argv[1], "-g", 2) == 0) {
    ga = argv[2];
  }

  if(ga) {
    int count = sscanf(ga, "%ux%u+%u+%u", &sw, &sh, &sx, &sy);
    printf("Looking at [%s] for geometry and found %d values.\n", ga, count);
  }

  fontsize = (int)((double)sw/800.0 * 24.0);
  
  printf("Going with geometry %ux%u+%u+%u\n", sw, sh, sx, sy);
  printf("Fontsize: %d\n", fontsize);
  
  char geo_dir[64];
  snprintf(geo_dir, 64, "gen/images/%ux%u", sw, sh);
  std::string geo_dir_str(geo_dir);

  SDL_Init(SDL_INIT_VIDEO);
  IMG_Init(IMG_INIT_PNG|IMG_INIT_TIF);
  TTF_Init();

  SDL_Window *w = SDL_CreateWindow("Unit Test", sx, sy, sw, sh, SDL_WINDOW_SHOWN);
  SDL_Renderer *r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  //  SDL_GL_SetSwapInterval(1);
  SDL_RendererInfo info;
  SDL_GetRendererInfo(r, &info);
  printf("Name: %s\n", info.name);
  printf("Flags: %u\n", info.flags);
  if(info.flags & SDL_RENDERER_SOFTWARE) printf("SDL_RENDERER_SOFTWARE\n");
  if(info.flags & SDL_RENDERER_ACCELERATED) printf("SDL_RENDERER_ACCELERATED\n");
  if(info.flags & SDL_RENDERER_PRESENTVSYNC) printf("SDL_RENDERER_PRESENTVSYNC\n");
  if(info.flags & SDL_RENDERER_TARGETTEXTURE) printf("SDL_RENDERER_TARGETTEXTURE\n");

  TTF_Font *f = TTF_OpenFont(findFont("DejaVuSansMono-Bold").c_str(), fontsize);

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

  uint32_t last_button_timestamp = 0;
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
	} else if(e.key.keysym.sym == SDLK_p) {
	  BOOL onoff = false;
	  CARD16 state;
	  DPMSInfo(xd, &state, &onoff);
	  if (!onoff) {
	    printf("DPMS disabled\n");
	  } else {
	    switch (state) {
	    case DPMSModeOn:        printf("DPMSModeOn\n"); break;    
	    case DPMSModeStandby:   printf("DPMSModeStandby\n"); break;    
	    case DPMSModeSuspend:   printf("DPMSModeSuspend\n"); break;    
	    case DPMSModeOff:       printf("DPMSModeOff\n"); break;    
	    default:	            printf("Unknown DPMS Mode\n");    
	    }
	  }
	}
      } else if(e.type == SDL_MOUSEBUTTONUP) {
	printf("Event SDL_MOUSEBUTTONUP (%d)@(%d,%d) at time %u\n", e.button.button, e.button.x, e.button.y, e.button.timestamp);
	if(e.button.x > sw*0.9 && e.button.y > sh*0.9 && e.button.timestamp - last_button_timestamp > 1000) {

	  printf("Setting DPMSForceLevel to DPMSModeOff\n");
	  last_button_timestamp = e.button.timestamp;
	  DPMSEnable(xd);
	  XSync(xd, false);
	  //	  usleep(100000);
	  DPMSForceLevel(xd, DPMSModeOff);
	  XSync(xd, false);
	}
	
      } else if(e.type == SDL_MOUSEBUTTONDOWN) {
	printf("Event SDL_MOUSEBUTTONDOWN (%d)@(%d,%d)\n", e.button.button, e.button.x, e.button.y);
      } else if(e.type == SDL_FINGERDOWN) {
	// printf("Event SDL_FINGERDOWN (%ld)@(%f,%f)\n", e.tfinger.touchId, e.tfinger.x, e.tfinger.y);
      }
    }

    SDL_SetRenderDrawColor(r, 0x0, 0x0, 0x0, 0x0);
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
      
      SDL_Surface *s = TTF_RenderText_Blended(f, buf, {0xcc,0x55,0x0,0x0});
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

  XCloseDisplay(xd);

  printf("Generated %zu frames, average fps: %f, max fps: %f\n", count, (double)count*1e9/(double)(1e9*(end_time.tv_sec - start_time.tv_sec) + (end_time.tv_nsec - start_time.tv_nsec)), 1e9*count/total_consumed);


  // Test screen brightness control
  // My current screen on the Pi doesn't support software
  // brightness control, sadly.
  
  // Test GPS connection
  /// @TODO

  Spacetime::destroy();
  
  exit(0);
}
