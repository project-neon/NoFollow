//this class will have enums of robot's state, alarms and beeps to make it default throughout the entire application
#include <stdint.h>
#include <math.h>
#include <helper_3dmath.h>

#include <Thread.h>

#ifndef TYPES_H
#define TYPES_H

enum RobotState{
  IDDLE = 0,
  ACTIVE = 1,
};

enum BeepState{
  UNDEF = -1,
  BEEP_NONE = 0,
  WARN = 1,
  ALARM = 2
};

struct Activity{
  const String title;
  Thread *thread;
};

#endif
