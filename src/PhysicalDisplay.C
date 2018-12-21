#include <error.h>
#include <string>
#include <thread>
#include <atomic>
#include <X11/Xlib.h>
#include <X11/extensions/dpms.h>

#include <PhysicalDisplay.H>

// Need to support:
// - forcing the display on and off
// - adjusting the backlight amount


int dpms_init() {
  Display *d = XOpenDisplay(NULL);
  DPMSEnable(d);
}

int dpms_done(Display *d) {
  XCloseDisplay(d);
}

int dpms_screen_on(Display *d) {
  DPMSForceLevel(d, DPMSModeOn);
}

int dpms_screen_off(Display *d) {
  DPMSForceLevel(d, DPMSModeOff);
}

int backlight_level(int l) {
  // /sys/class/backlight/{acpi_video0,intel_backlight}/brightness
  // 0 - max_brightness
  // => actual_brightness
  // xbacklight -display $DISPLAY -set $PERCENT

}

