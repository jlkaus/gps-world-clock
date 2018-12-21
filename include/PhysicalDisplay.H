#ifndef _PHYSICALDISPLAY_H
#define _PHYSICALDISPLAY_H

#include <atomic>
#include <thread>

struct _XDisplay;
typedef struct _XDisplay Display;

class PhysicalDisplay {
public:
  // Initialize the connection to the display, and start the thread that
  // controls the screen state and brightness level.  Interacts with Spacetime to compute the brightness levels during the day.
  // - night_brightness - brightness level to use at night (after sunset, before sunrise) 
  // - twilight_brightness - brightness level to use just after sunrise and just before sunset
  // - noon_brightness - brightness level to use at noon
  // - touch_brightness - brightness level to use after being touched
  // - touch_delay - number of seconds at touch_brightness after being touched
  // - on_time - number of seconds into the day at which to turn on the screen
  // - off_time - number of seconds into the day at which to turn off the screen
  PhysicalDisplay(double night_brightness = 1.0,
		  double twilight_brightness = 0.75,
		  double noon_brightness = 0.5,
		  double touch_brightness = 1.0,
		  double touch_delay = 20.0,
		  double on_time = 7.0*60.0*60.0,
		  double off_time = 20.0*60.0*60.0);
  ~PhysicalDisplay();

  // Called externally when the screen is touched.
  // This will cause the screen to turn on and brightness
  // to go all the way up, for some amount of time after
  // the touch.  Each time its touched, that resets the
  // timer to re-dim.
  // After that time elapses, the screen state returns
  // to the state dictated by the current time of day.
  void notifyTouch();

private:
  // Internal utility function to turn on the screen
  void on();

  // Internal utility function to turn off the screen
  void off();

  // Internal utility function to set the current backlight level to level.
  void backlight(double level);

  // Main thread function
  void run();

  // Compute what level of brightness should be used right now,
  // given the settings we were constructed with, the current time,
  // and the times of true local twilight and noon.
  // state is set to true if it should be on, false if it should be
  // off.
  double computeBrightness(bool &state) const;
  
private:
  std::atomic<bool> terminate;
  std::atomic<Display *> d;
  
  std::atomic<double> lastTouch;
  
  double night_brightness;   // relative brightness during true local dark
  double twilight_brightness; // relative brightness just before sunset and just after sunrise
  double noon_brightness;    // relative brightness at true local noon
  double touch_brightness;   // relative brightness when recently touched
  double touch_delay;   // seconds since lastTouch at which to return to normal
  double on_time;  // measured as seconds since local timezone midnight
  double off_time; // measured as seconds since local timezone midnight

  // Should be the last data member, so all other items are constructed
  // before the thread starts.
  std::thread self;
};

#endif
