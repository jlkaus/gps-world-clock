#include <error.h>
#include <string>
#include <thread>
#include <atomic>
#include <cmath>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/extensions/dpms.h>

#include <PhysicalDisplay.H>
#include <Spacetime.H>

PhysicalDisplay::PhysicalDisplay(double night_brightness,
				 double twilight_brightness,
				 double noon_brightness,
				 double touch_brightness,
				 double touch_delay,
				 double on_time,
				 double off_time):
  terminate(false),
  lastTouch(0.0),
  night_brightness(night_brightness),
  twilight_brightness(twilight_brightness),
  noon_brightness(noon_brightness),
  touch_brightness(touch_brightness),
  touch_delay(touch_delay),
  on_time(on_time),
  off_time(off_time),
  self(&PhysicalDisplay::run, this)
{
  // Do no work here.  Let the thread run function initialize the display
}

PhysicalDisplay::~PhysicalDisplay() {
  // Request our thread to terminate, and wait for it
  terminate.store(true, std::memory_order_release);
  self.join();
}

void PhysicalDisplay::notifyTouch() {
  // get the current time of day and record it
  double nowf = Spacetime::getUTC();
  lastTouch.store(nowf, std::memory_order_release);  
}

void PhysicalDisplay::on() {
  // This check is just for my own sanity
  // There is a race condition if this function is
  // called off the screen control thread.
  if(d) {
    DPMSForceLevel(d, DPMSModeOn);
  }
}

void PhysicalDisplay::off() {
  // This check is just for my own sanity.
  // There is a race condition if this function is
  // called off the screen control thread.
  if(d) {
    DPMSForceLevel(d, DPMSModeOff);
  }
}

void PhysicalDisplay::backlight(double level) {
  // /sys/class/backlight/{acpi_video0,intel_backlight}/brightness
  // 0 - max_brightness
  // => actual_brightness
  // xbacklight -set $PERCENT
  std::string cmd("/usr/bin/xbacklight -set ");
  cmd += std::to_string(level * 100.0);  
  system(cmd.c_str());
}

void PhysicalDisplay::run() {
  // Open a display handle
  d.store(XOpenDisplay(NULL), std::memory_order_release);

  // Turn the screen on and to full brightness
  on();
  backlight(1.0);
  bool old_state = true;
  double old_level = 1.0;

  // Loop forever until asked to quit
  while(!terminate.load(std::memory_order_acquire)) {
    bool state = true;
    double level = computeBrightness(state);

    if(state != old_state) {
      if(state) {
	// turn it on!
	on();
	old_state = true;
	backlight(level);
	old_level = level;
      } else {
	// turn it off
	off();
	old_state = false;
      }
    } else if((state == true) && (std::fabs(level - old_level) > 0.01)) {
      // big enough change in backlight level to adjust
      backlight(level);
      old_level = level;
    }

    // just sleep for a quick bit to avoid constantly computing brightnesses
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }  
  
  // Turn the screen back on and to 100%, just in case
  on();
  backlight(1.0);
  
  // Close the display handle
  Display *tmp_d = d.load(std::memory_order_acquire);
  d.store(nullptr, std::memory_order_release);
  XCloseDisplay(tmp_d);
}

double PhysicalDisplay::computeBrightness(bool &state) const {
  // figure out the current utc CLOCK_REALTIME time as a float.
  double nowf = Spacetime::getUTC();
  // figure out the current timezone time of day (seconds since midnight) as a float.
  double tod = Spacetime::getTZ_TOD();
  // figure out our day state (morning, astro/nautical/civil dawn, forenoon, afternoon, civil/nautical/astro dusk, evening)
  Spacetime::DayState day_state = Spacetime::getLocalDayState();

  // Some helpers for true local times
  double local_now = Spacetime::getTL_TOD();
  double local_noon = Spacetime::getTL_NOON();
  double local_sunrise = Spacetime::getTL_SUNRISE();
  double local_sunset = Spacetime::getTL_SUNSET();

  double fraction = 0.0;
  double level = 0.0;
  
  if(nowf < lastTouch.load(std::memory_order_acquire) + touch_delay) {
    // Recently touched, so make sure its on and at the appropriate brightness
    state = true;
    level = touch_brightness;
  } else if(tod < on_time || tod > off_time) {
    // We're currently outside of today's on-time, so just turn it off
    state = false;
    level = 0.0;
  } else {
    // Otherwise, it should be on, but we need to work harder to compute the brightness level.
    state = true;

    switch(day_state) {
    case Spacetime::MORNING:
    case Spacetime::EVENING:
      // Counts as nighttime, so set the level to night_brightness
      level = night_brightness;
      break;
    case Spacetime::ASTRONOMICAL_TWILIGHT_MORNING:
    case Spacetime::NAUTICAL_TWILIGHT_MORNING:
    case Spacetime::CIVIL_TWILIGHT_MORNING:
    case Spacetime::CIVIL_TWILIGHT_EVENING:
    case Spacetime::NAUTICAL_TWILIGHT_EVENING:
    case Spacetime::ASTRONOMICAL_TWILIGHT_EVENING:
      // Counts as twilight, so set the level to twilight_brightness
      level = twilight_brightness;
      break;
    case Spacetime::FORENOON:
      // Daylight, so compute the relative fraction of the way between
      // sunrise and noon, and set the level to that fraction between
      // twilight_brightness and noon_brightness.
      fraction = (local_now - local_sunrise)/(local_noon - local_sunrise);
      level = fraction * (noon_brightness - twilight_brightness) + twilight_brightness;
      
      break;
    case Spacetime::AFTERNOON:
      // Daylight, so compute the relative fraction of the way between
      // sunrise and noon, and set the level to that fraction between
      // twilight_brightness and noon_brightness.
      fraction = (local_sunset - local_now)/(local_sunset - local_noon);
      level = fraction * (noon_brightness - twilight_brightness) + twilight_brightness;

      break;
    }
  }

  return level;
}
  

