#ifndef _SPACETIME_H
#define _SPACETIME_H

class Spacetime {
public:
  enum DayState {
    MORNING = 0,
    EVENING,
    ASTRONOMICAL_TWILIGHT_MORNING,
    NAUTICAL_TWILIGHT_MORNING,
    CIVIL_TWILIGHT_MORNING,
    CIVIL_TWILIGHT_EVENING,
    NAUTICAL_TWILIGHT_EVENING,
    ASTRONOMICAL_TWILIGHT_EVENING,
    FORENOON,
    AFTERNOON
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

#endif
