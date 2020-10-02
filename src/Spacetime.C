#include <error.h>
#include <string>
#include <thread>
#include <atomic>
#include <gps.h>

#include <Spacetime.H>

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

Spacetime::Spacetime(): terminate(false), m(), self(&Spacetime::run, this)
{}

Spacetime::~Spacetime() {
  terminate.store(true, std::memory_order_release);
  self.join();
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
  m.lock();
  gps_open(GPSD_SHARED_MEMORY, NULL, &gd);
  m.unlock();
  
  while(!terminate.load(std::memory_order_acquire)) {
    m.lock();
    gps_read(&gd);
    m.unlock();

    // sleep for some time before next fetch.  Make this configurable?
    // Most GPS units have new data on the order of once per second.
    // We'll want to query more frequently than that by a few times to make sure
    // we don't miss a transition.
    // Also, making this really long would delay shutdown.
    // For now, we'll read 10 times a second or so.
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  m.lock();
  gps_close(&gd);
  m.unlock();
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




