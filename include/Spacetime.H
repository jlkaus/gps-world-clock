#ifndef _SPACETIME_H
#define _SPACETIME_H

#include <gps.h>
#include <vector>
#include <time.h>
#include <stdint.h>

class SMoment {
public:
  time_t s;
  long ns;

  static SMoment nowSystem_UTC();
  static SMoment nowGPS_UTC();
  static SMoment nowMono();
  static SMoment PosixfromJD(const DMoment &rhs);
  static SMoment PosixfromJD2000(const DMoment &rhs);
  static SMoment TTfromUTC(const SMoment &rhs);
  static SMoment TAIfromUTC(const SMoment &rhs);
  static SMoment GPSfromUTC(const SMoment &rhs);

  DMoment JDfromPosix() const;
  DMoment JD2000fromPosix() const;

  void sleepUntilSystemUTC() const;
  void sleepUntilMono() const;
  
  SMoment operator+(const SDuration &rhs) const;
  SMoment operator-(const SDuration &rhs) const;
  SDuration operator-(const SMoment &rhs) const;
  SMoment operator+(const DDuration &rhs) const;
  SMoment operator-(const DDuration &rhs) const;
};

class SDuration {
public:
  time_t s;
  long ns;

  SDuration(const DDuration &rhs);
  
  SDuration operator+(const SDuration &rhs) const;
  SDuration operator-(const SDuration &rhs) const;
  SDuration operator+(const DDuration &rhs) const;
  SDuration operator-(const DDuration &rhs) const;
  STimestamp operator+(const STimestamp &rhs) const;

  void sleepFor() const;
};

class DMoment {
public:
  int64_t i;
  double f;

  static DMoment JDfromPosix(const SMoment &rhs);
  static DMoment JD2000fromPosix(const SMoment &rhs);

  SMoment PosixfromJD() const;
  SMoment PosixfromJD2000() const;
  DMoment JD2000fromJD() const;
  DMoment JDfromJD2000() const;
  DMoment JD2000_TT_from_UTC() const;
  DMoment JD2000_UTC_from_TT() const;
  
  DMoment operator+(const DDuration &rhs) const;
  DMoment operator-(const DDuration &rhs) const;
  DDuration operator-(const DMoment &rhs) const;
  DMoment operator+(const SDuration &rhs) const;
  DMoment operator-(const SDuration &rhs) const;
};

class DDuration {
public:
  int64_t i;
  double f;

  DDuration(const SDuration &rhs);
  
  DDuration operator+(const DDuration &rhs) const;
  DDuration operator-(const DDuration &rhs) const;
  DDuration operator+(const SDuration &rhs) const;
  DDuration operator-(const SDuration &rhs) const;
  DTimestamp operator+(const DTimestamp &rhs) const;
};

class Location {
public:
  double longitude;
  double latitude;

  static Location here();
  static Location fromCoordinateSpace(size_t x, size_t y, size_t width, size_t height);
};

static Spacetime *st = nullptr;

class Spacetime {
public:
  enum DayState {
    MORNING = 0,
    DAWN,
    FORENOON,
    AFTERNOON,
    DUSK,
    EVENING
  };

public:
  static SDuration getLeapSecondsAdded();
  static SDuration getLocalTimezoneOffset();
  static double getGMSTfromPosixUTC(STimestamp posix_utc);
  static double getGMSTfromJD2000UTC(DTimestamp jd2000_utc);
  static DTimestamp getJD2000UTCfromGMST(DTimestamp jd2000_utc, double);
  static double getLMSTfromGMST(double gmst, Location l);
  static STimestamp getLocalTimeFromUTC(STimestamp posix_utc, SDuration tz_offset);
  static STimestamp getLocalApparentTimeFromUTC(STimestamp posix_utc, Location l);
  
  static Spacetime::DayState getLocalDayState(STimestamp posix_utc, Location l);
  static SDuration getLocalDayLength(STimestamp posix_utc, Location l);  // if we're in the day, get current day length, if in the night, get next day length
  static SDuration getLocalNightLength(STimestamp posix_utc, Location l); // if we're in the night, get current night length, if in the day, get next night length
  
  static double getSolarDeclination(DTimestamp jd2000_utc);
  static double getRightAscension(DTimestamp jd2000_utc);
  static double getLocalSolarElevation(DTimestamp jd2000_utc, Location l);
  static double getLocalHourAngle(DTimestamp jd2000_utc, Location l);
  
  static double getEclipticObliquity(DTimestamp jd2000_utc);
  static double getEclipticLongitude(DTimestamp jd2000_utc);

  static std::vector<double> getGMSTOfLocalSolarElevation(DTimestamp jd2000_utc, Location l, double a);
  static STimestamp getUTCOfLocalMidnight(STimestamp utc, Location l, int index);
  static STimestamp getUTCOfLocalNoon(STimestamp utc, Location l, int index);
  static STimestamp getUTCOfLocalSunrise(STimestamp utc, Location l, int index);
  static STimestamp getUTCOfLocalSunset(STimestamp utc, Location l, int index);
  static STimestamp getUTCOfLocalDawn(STimestamp utc, Location l, int index);
  static STimestamp getUTCOfLocalDusk(STimestamp utc, Location l, int index);

  int getFixType() const;
  double getAltitude() const;
  double getTError() const;
  double getXError() const;
  double getYError() const;
  double getVError() const;
  std::vector<int> getSatellitesUsed() const;
  std::vector<std::tuple<int, int, int, double> > getSatelliteInfo() const;
  Location getLocation() const;
  STimestamp getGPSTimestamp() const;

  Spacetime();
  ~Spacetime();

  void run();
  
  static void init();
  static void destroy();
  static const Spacetime *getSingleton() {return st;}
  

  std::atomic<bool> terminate;
  mutable std::mutex m;
  struct gps_data_t gd;
  std::thread self;
};

// index can be -1 or 1, for previous event, and next event, respectively.
// Choosing indices larger in magnitude than 1 would theoretically correspond to the
// n-th next/previous such event, but due to the use of the current timestamp in
// calculating these event times, we'd internally have to advance/retreat the current time, and re-calculate.
// Choosing an index of 0 is invalid, but could be implemented to act like 1.

// Event solar elevation angles
// previous noon maximum
// previous sunset 0
// previous dusk -18
// previous midnight minimum
// previous dawn -18
// previous sunrise 0
// next noon maximum
// next sunset 0
// next dusk -18
// next midnight minimum
// next dawn -18
// next sunrise 0




#endif
