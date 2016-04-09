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

	// Initialize pins
	static void init();

	// Set power of both DC motors
	static void setPower(float m1, float m2);

    // Set angle of Servo motors
	static void setAngle(float a);

	// Set state as Iddle (Turn off motors)
	static void stop();
};

#endif