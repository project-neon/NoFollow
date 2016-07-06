//this class will contain all PID logic and processing
#include "_config.h"
#include "_types.h"

#include "robot.h"

#ifndef pid_H
#define pid_H
class PID{
public:

  float desired;     //< set point

	float prevError;    //< previous error
	float integ;        //< integral

	float kp;           //< proportional gain
	float ki;           //< integral gain
	float kd;           //< derivative gain

	float iLimit;       //< integral limit

  PID(float kp, float ki, float kd, float _ilimit);
  void setSetPoint(float _setPoint);
  float update(float input, float dt);
	void clear();
};

#endif
