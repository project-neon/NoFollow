//this class will contain all comands to use IMU sensor in the robot

#include <Wire.h>
#include <Thread.h>
#include <MPU6050_6Axis_MotionApps20.h>

#include "_types.h"

#ifndef IMUThread_H
#define IMUThread_H

class IMUThread: public Thread{

protected:
	MPU6050 mpu;

	// Status of DMP (Enabled/Not enabled)
	bool dmpEnabled;

	// Last packed received
	uint8_t packet[84];

	uint8_t fifoCount;
	int interruptStatus;

public:
	bool newData;

	IMUThread();

	bool init();

	// Return if the Thread should be runned or not
	bool shouldRun(unsigned long time);

	// run() Method is overrided
	void run();
};

extern IMUThread imu;

#endif
