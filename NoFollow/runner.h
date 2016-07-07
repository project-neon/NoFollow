//this class will have the motors configuration, monitoration and control
#include "_config.h"
#include "_types.h"

#ifndef RUNNER_H
#define RUNNER_H

class Runner{
public:
  static bool invalidated;

  static void init();
  static void exit();
  static void setActivity(Activity *activity);
};

#endif
