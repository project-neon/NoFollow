// this class will have the robot states and alarms models
#include "_config.h"
#include "_types.h"

#include <Thread.h>
#include <ThreadController.h>

#ifndef ROBOT_H
#define ROBOT_H


class Robot{
public:


  // ====================================
  //           INITIALIZERS
  // ====================================

  static void init();


  // ====================================
  //       BEEP and Voltage states
  // ====================================

  /*
    Battery voltage (in volts, ohhh!)
  */
  static float vbat;

  /*
    Boolean flag indicating Active stabilizing/Power on motors
  */
  static RobotState state;

  /*
    Set's the state to the robot
  */
  static void setState(RobotState _state);

  /*
    Beeper flag/setter
  */
  static int beepTimes;
  static int beepInterval;
  static BeepState beepState;

  static void setBeep(BeepState state);
  static void doBeep(int _times, int interval);


  // ====================================
  //         ATTITUDE/ORIENTATION
  // ====================================

  static Quaternion q;
  static VectorFloat gravity;
  static float ypr[3];

};

extern ThreadController system;

#endif
