#ifndef _SPACETIME_H
#define _SPACETIME_H

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
  static double getUTC();
  static double getTZ_TOD();

  static double getTL_TOD();
  static double getTL_NOON();
  static double getTL_SUNRISE();
  static double getTL_SUNSET();
  static double getTL_MIDNIGHT();
  static double getTL_NEXT_MIDNIGHT();
  static Spacetime::DayState getLocalDayState();


};

void getUTC(struct timespec *tsUTC);
int getLeapSecondCount();
double getLatitude();
double getLongitude();
double getLatitudeFromCoordinate(uint32_t x, uint32_t max_x);
double getLongitudeFromCoordinate(uint32_t y, uint32_t max_y);
double getDeclination(struct timespec tsUTC);
double getRightAscension(struct timespec tsUTC);
double getEclipticObliquity(struct timespec tsUTC);
double getEclipticLongitude(struct timespec tsUTC);
double getJ2000DayNumber(struct timespec ts);
double getGMST(struct timespec tsUTC);
void getTTFromUTC(struct timespec tsUTC, struct timespec *tsTT);
void getGPSFromUTC(struct timespec tsUTC, struct timespec *tsGPS);
void getTAIFromUTC(struct timespec tsUTC, struct timespec *tsTAI);
void getLTMFromUTC(struct timespec tsUTC, double longitude, struct timespec *tsLTM);
void getLTFromUTC(struct timespec tsUTC, struct tzinfo_t tz, struct timespec *tsLT);
double getLMST(struct timespec tsUTC, double longitude);
double getLocalHourAngle(double longitude, struct timespec tsUTC);
double getSolarElevation(double longitude, double latitude, struct timespec tsUTC);
void getTimeOfLocalMidnight(double longitude, double latitude, int index, struct timespec tsUTC, struct timespec *tsUTCLocalEvent);
void getTimeOfLocalDawn(double longitude, double latitude, int index, struct timespec tsUTC, struct timespec *tsUTCLocalEvent);
void getTimeOfLocalSunrise(double longitude, double latitude, int index, struct timespec tsUTC, struct timespec *tsUTCLocalEvent);
void getTimeOfLocalNoon(double longitude, double latitude, int index, struct timespec tsUTC, struct timespec *tsUTCLocalEvent);
void getTimeOfLocalSunset(double longitude, double latitude, int index, struct timespec tsUTC, struct timespec *tsUTCLocalEvent);
void getTimeOfLocalDusk(double longitude, double latitude, int index, struct timespec tsUTC, struct timespec *tsUTCLocalEvent);
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
