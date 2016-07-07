#include <Thread.h>

#include "_config.h"
#include "_types.h"

#include "motors.h"

#ifndef ACTIVITIES_H
#define ACTIVITIES_H

extern Thread activityLineReader;
extern Thread activityLineFollower;
extern Thread activityActuators;
extern Thread activityCalibrateSteering;

#endif
