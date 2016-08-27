//this class will have the motors configuration, monitoration and control
#include "_config.h"
#include "_types.h"

#include "robot.h"

#ifndef MOTORS_H
#define MOTORS_H

class Motors{
public:
	// Average of both motors
	static float avgSpeed;
  static float avgAngle;

  // Steering configurations (Loaded from EEPROM)
  static int8_t steeringPhase;
  static int8_t steeringConverge;

	// Initialize pins
	static void init();

  // Set the steering for the wheels
  static void setSteering(float steer, bool activate = true);

	// Set power of both DC motors
	static void setPower(float m1, float m2);

	// Set state as Iddle (Turn off motors)
	static void stop();
};

extern int MotorsSteeringPhase;
extern int MotorsSteeringConverge;

#endif
