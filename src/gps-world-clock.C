#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stderr.h>
#include <error.h>
#include <string>
#include <thread>
#include <atomic>
#include <X11/Xlib.h>
#include <X11/extensions/dpms.h>
#include <gps.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <argp.h>



int main(int argc, char *argv[]) {





}

int load_image() {
  IMG_Load();
}

int load_font() {
  TTF_OpenFont();
}

int dpms_init() {
  d = XOpenDisplay(NULL);
  DPMSEnable(d);
}

int dpms_done() {
  DPMSForceLevel(d, DPMSModeAuto);
  XCloseDisplay(d);
}

int dpms_screen_on() {
  DPMSForceLevel(d, DPMSModeOn);
}

int dpms_screen_off() {
  DPMSForceLevel(d, DPMSModeOff);
}

int backlight_level(int l) {
  // /sys/class/backlight/{acpi_video0,intel_backlight}/brightness
  // 0 - max_brightness
  // => actual_brightness
  // xbacklight -display $DISPLAY -set $PERCENT

}

int gps_init() {
  struct gps_data_t *gd;
  gps_open(GPSD_SHARED_MEMORY, NULL, gd);
}

int gps_done() {
  gps_close(gd);
}

int gps_update() {
  gps_read(gd);

  // gps_data_t:
  //   gps_mask_t set
  //   timestamp_t online
  //   gps_fix_t fix
  //   int status // NoFix=0, Fix=1, DGPSFix=2
  //   int satellites_used
  //   int used[] // PRNs
  //   timestamp_t skyview_time
  //   int satellites_visible
  //   int PRN[]
  //   int elevation[]
  //   int azimuth[]
  //   double SS[]   // SNR
  // gps_fix_t:
  //   timestamp_t time
  //   int mode // NotSeen=0, None=1, 2D=2, 3D=3
  //   double ept
  //   double latitude, epy, longitude, epx, altitude, epv
  //   double track, epd, speed, eps, climb, epc
  // gps_mask_t:
  //   ONLINE_SET
  //   TIME_SET
  //   TIMERR_SET
  //   LATLON_SET
  //   HERR_SET
  //   VERR_SET
  //   ALTITUDE_SET
  //   ATTITUDE_SET
  //   SPEED_SET
  //   SPEEDERR_SET
  //   TRACK_SET
  //   TRACKERR_SET
  //   CLIMB_SET
  //   CLIMBERR_SET
  //   STATUS_SET
  //   MODE_SET
  //   SATELLITE_SET

}

