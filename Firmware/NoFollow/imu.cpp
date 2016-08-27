//That is the version v0.1 of the IMU sensor

#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050_6Axis_MotionApps20.h>

#include "_config.h"
#include "_types.h"

#include "robot.h"
#include "imu.h"

IMUThread::IMUThread(): Thread(){
}

bool IMUThread::init(){
	bool devStatus;

	Serial.println("IMU::init");

	// Fastwire::setup(100, true);
	Wire.begin();
	TWBR = ((16000000L / 400000L) - 16) / 2;
	delay(1);
	mpu.initialize();

	if(!mpu.testConnection()){
		Serial.println("MPU6050 connection failed");
		return false;
	}

  //disabling the DMP (that is like a PID inside the IMU sensor)
	mpu.setDMPEnabled(false);
	mpu.resetDMP();
	// Reset IMU
	mpu.reset();
	delay(50);

	// Activate MPU6050
	mpu.setSleepEnabled(false);

	// load and configure the DMP
	devStatus = mpu.dmpInitialize();

	// supply your own gyro offsets here, scaled for min sensitivity
	mpu.setXGyroOffset(83);
	mpu.setYGyroOffset(-3);
	mpu.setZGyroOffset(48);
	mpu.setZAccelOffset(1974); // 1688 factory default for my test chip

	// make sure it worked (returns 0 if so)
	if (devStatus != 0) {
		// ERROR!
		// 1 = initial memory load failed
		// 2 = DMP configuration updates failed
		// (if it's going to break, usually the code will be 1)
		Serial.print("DMP Initialization failed (code ");
		Serial.print(devStatus);
		Serial.println(")");
	}

	dmpEnabled = false;

	return true;
}

bool IMUThread::shouldRun(unsigned long time){
	if(enabled != dmpEnabled){
		Serial.println(enabled ? "IMU ON" : "IMU OFF");
		mpu.setDMPEnabled(enabled);
		dmpEnabled = enabled;
	}

	if(!Thread::shouldRun(time))
		return false;

	// Check for new Packet
	interruptStatus = mpu.getIntStatus();
	fifoCount = mpu.getFIFOCount();
	return (fifoCount) >= 42;
}

int outOfSyncs = 0;
void IMUThread::run(){
	// if(fifoCount > 42 * 3){
	// 	Serial.print("Fifo out sync ");
	// 	Serial.println(fifoCount);
	// }

	if (interruptStatus & 0x02) {
		runned();

		// read a packet from FIFO
		mpu.getFIFOBytes(packet, 42);
		// mpu.resetFIFO();

		// Convert and save state to Drone
		mpu.dmpGetQuaternion(&Robot::q, packet);
		mpu.dmpGetGravity(&Robot::gravity, &Robot::q);
		mpu.dmpGetYawPitchRoll(Robot::ypr, &Robot::q, &Robot::gravity);

		newData = true;
	}

	if (fifoCount >= 42 * 3 || interruptStatus & 0x10) {
		// reset so we can continue cleanly
		mpu.resetFIFO();
	}
}
