#include <error.h>
#include <string>
#include <thread>
#include <atomic>
#include <gps.h>
#include <assert.h>
//#define _GNU_SOURCE
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/timex.h>
#include <math.h>
#include <string.h>

#include "Spacetime.H"

SMoment::SMoment(double t) {
  double a;
  double b = modf(t, &a);
  s = (time_t)a;
  ns = (long)(b * 1e9);
}

SMoment::SMoment(time_t a, long b) {
  s = a;
  ns = b;
  renormalize();
}

SMoment SMoment::nowSystem_UTC() {
  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  return SMoment(ts.tv_sec, ts.tv_nsec);
}

SMoment SMoment::nowGPS_UTC() {
  return Spacetime::getSingleton()->getGPSTimestamp();
}

SMoment SMoment::nowMono() {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return SMoment(ts.tv_sec, ts.tv_nsec);
}

//SMoment SMoment::PosixfromJD(const DMoment &rhs) {
//}

//SMoment SMoment::PosixfromJD2000(const DMoment &rhs) {
//}

//SMoment SMoment::TTfromUTC(const SMoment &rhs) {
//}

//SMoment SMoment::TAIfromUTC(const SMoment &rhs) {
//}

//SMoment SMoment::GPSfromUTC(const SMoment &rhs) {
//}

//DMoment SMoment::JDfromPosix() const {
//}

//DMoment SMoment::JD2000fromPosix() const {
//}

//void SMoment::sleepUntilSystemUTC() const {
//}

//void SMoment::sleepUntilMono() const {
//}
  
SMoment SMoment::operator+(const SDuration &rhs) const {
  return SMoment(s + rhs.s, ns + rhs.ns);
}

SMoment SMoment::operator-(const SDuration &rhs) const {
  return SMoment(s - rhs.s, ns - rhs.ns);
}

SDuration SMoment::operator-(const SMoment &rhs) const {
  return SDuration(s - rhs.s, ns - rhs.ns);
}

//SMoment SMoment::operator+(const DDuration &rhs) const {
//  return SMoment(s + rhs.s, ns + rhs.ns);
//}

//SMoment SMoment::operator-(const DDuration &rhs) const {
//  return SMoment(s - rhs.s, ns - rhs.ns);
//}

void SMoment::renormalize() {
  while(ns >= 1e9) {
    ns -= 1e9;
    s += 1;
  }

  while(ns <= -1e9) {
    ns += 1e9;
    s -= 1;
  }

  // So now, -1e9 < ns < 1e9
  // if s < 0: ns should be negative, too
  if(s < 0) {
    if(ns > 0) {
      s += 1;
      ns -= 1e9;
    }
  }

  // if s = 0, the sign of ns is fine

  // if s > 0: ns needs to be positive
  if(s > 0) {
    if(ns < 0) {
      s -= 1;
      ns += 1e9;
    }
  }
  
}

void SDuration::renormalize() {
  while(ns >= 1e9) {
    ns -= 1e9;
    s += 1;
  }

  while(ns <= -1e9) {
    ns += 1e9;
    s -= 1;
  }

  // So now, -1e9 < ns < 1e9
  // if s < 0: ns should be negative, too
  if(s < 0) {
    if(ns > 0) {
      s += 1;
      ns -= 1e9;
    }
  }

  // if s = 0, the sign of ns is fine

  // if s > 0: ns needs to be positive
  if(s > 0) {
    if(ns < 0) {
      s -= 1;
      ns += 1e9;
    }
  }
  
}

SDuration::SDuration(time_t a, long b) {
  s = a;
  ns = b;
  renormalize();
}

Location Location::here() {
  return Spacetime::getSingleton()->getLocation();
}

Location Location::fromCoordinateSpace(size_t x, size_t y, size_t width, size_t height) {
  // assume equator is half-way through the image, as is the prime meridian.
  double equator_y = height/2.0;
  double prime_meridian_x = width/2.0;
  double pixels_per_long = width / 360.0;
  double pixels_per_lat = height / 180.0;
  return Location((size_t)(((double)x - prime_meridian_x)/pixels_per_long),
		  (size_t)(-((double)y - equator_y)/pixels_per_lat));
}

// Support a thread to update GPS data fairly consistently
// If no GPS available, use system time & latitude/longitude that was
//   passed in as parameters, or default to South Ridge or something?
// Support retrieving:
// - current UTC timestamp
// - current local timezone timestamp
// - current true local timestamp
// - current location
// - current satellite info (if relevant)
// - current location uncertainty
// Support calculation of:
// - terminator equations
// - local dawn/sunrise/transit/sunset/dusk/midnight (including astronomical, civil, and nautical twilight) 
// - length of day and night
// - hour/watch divisions
// - point of sun overhead
// - point of moon overhead
// - local moonrise/transit/moonset


Spacetime::Spacetime(): terminate(false), valid(false), m(), self(&Spacetime::run, this)
{}

Spacetime::~Spacetime() {
  terminate.store(true, std::memory_order_release);
  self.join();
}


SDuration Spacetime::getCurrentTAIOffset() {
  // OK, I guess we have to get this from adjtimex() ...
  // If its 0, assume the kernel tai_offset isn't set yet,
  // then I guess we have to go with the current offset, 37...
  struct timex adjt;
  memset(&adjt, 0, sizeof(struct timex));
  int rc = adjtimex(&adjt);
  if(rc > 0 && rc != TIME_ERROR && adjt.tai > 0) {
    return SDuration(adjt.tai, 0);
  } else {
    return SDuration(37, 0);
  }
}

SDuration Spacetime::getLocalTimezoneOffset() {
  struct tm bdt;
  time_t ct = SMoment::nowSystem_UTC().s;
  localtime_r(&ct, &bdt);

  return SDuration(bdt.tm_gmtoff, 0);

}

void Spacetime::init() {
  assert(st == nullptr);
  st = new Spacetime();
}

void Spacetime::destroy() {
  Spacetime *tmp_s = st;
  assert(tmp_s != nullptr);
  st = nullptr;

  delete tmp_s;
}

void Spacetime::run() {
  pthread_setname_np(self.native_handle(), "SPACETIME");
  m.lock();
  int rc = gps_open(GPSD_SHARED_MEMORY, NULL, &gd);
  if(rc == 0) {
    valid.store(true, std::memory_order_release);
  }
  m.unlock();
  
  while(!terminate.load(std::memory_order_acquire)) {
    if(valid.load(std::memory_order_acquire)) {
      m.lock();
      gps_read(&gd, NULL, 0);
      m.unlock();
    }
    
    // sleep for some time before next fetch.  Make this configurable?
    // Most GPS units have new data on the order of once per second.
    // We'll want to query more frequently than that by a few times to make sure
    // we don't miss a transition.
    // Also, making this really long would delay shutdown.
    // For now, we'll read 10 times a second or so.
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  if(valid.load(std::memory_order_acquire)) {
    m.lock();
    valid.store(false, std::memory_order_release);
    gps_close(&gd);
    m.unlock();
  }
}

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
int Spacetime::getFixType() const {
  int type = 0;
  if(valid.load(std::memory_order_acquire)) {
    m.lock();
    type = gd.fix.mode;
    m.unlock();
  }
  return type;
}

double Spacetime::getAltitude() const {
  double v = 0.0;
  if(valid.load(std::memory_order_acquire)) {
    m.lock();
    if(gd.fix.mode > 1) {
      v = gd.fix.altitude;
    }
    m.unlock();
  }
  return v;
}

//double Spacetime::getTError() const {
//}

//double Spacetime::getXError() const {
//}

//double Spacetime::getYError() const {
//}

//double Spacetime::getVError() const {
//}

//std::vector<int> Spacetime::getSatellitesUsed() const {
//}

//std::vector<std::tuple<int, int, int, double> > Spacetime::getSatelliteInfo() const {
//}

Location Spacetime::getLocation() const {
  Location l(DEFAULT_LONGITUDE, DEFAULT_LATITUDE);
  if(valid.load(std::memory_order_acquire)) {
    m.lock();
    if(gd.fix.mode > 1) {
      l.longitude = gd.fix.longitude;
      l.latitude = gd.fix.latitude;
    }
    m.unlock();
  }
  return l;
}

SMoment Spacetime::getGPSTimestamp() const {
  SMoment n;
  bool got_gps_time  = false;
  if(valid.load(std::memory_order_acquire)) {
    m.lock();
    if(gd.fix.mode > 1) {
      double l = gd.fix.time;
      n = SMoment(l);
      got_gps_time = true;
    }
    m.unlock();
  }

  if(!got_gps_time) {
    n = SMoment::nowSystem_UTC();
  }
  
  return n;
}





