// this class will contain the mais code that's called by the robot
//It will initiate the diferents robot's functions in the propper and logical order
// System Includes

// External Libraries
// #include <SPI.h>
#include <SPI.h>
#include <Wire.h>
#include <Servo.h>
#include <I2Cdev.h>
#include <Thread.h>
#include <EEPROM.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// #include <RH_NRF24.h>
#include <ThreadController.h>
// #include <RHReliableDatagram.h>
#include <MPU6050_6Axis_MotionApps20.h>


// Custom Util
#include "_types.h"
#include "_config.h"

// Modules
// #include "imu.h"
#include "pid.h"
#include "robot.h"
#include "motors.h"
#include "runner.h"
#include "interface.h"
#include "lineReader.h"
#include "activities.h"
// #include "controller.h"


void setup(){

  // Initialize Robot state and I/O Pins
  Robot::init();

  // Initialize Attitude/Orientation module (initializes MPU6050 internally)
  // Attitude::init();

  // Initialize Interface Display/Buttons/Serial debug
  Interface::init();

  // Initalize Line Sensors
  LineReader::init();

  // Initalize Motors (object)
  Motors::init();

  // Initialize Runner (Manages program execution)
  Runner::init();

  // Initialize Motor controllers (with PID's)
  // Controller::init();

}

// long start, end;
int pos = 0;
void loop(){
  // if(Interface::readButton() == STICK_RIGHT){
  //   pos++;
  //   Motors::setSteering(pos);
  //   delay(50);
  // }
  //
  // if(Interface::readButton() == STICK_LEFT){
  //   pos--;
  //   Motors::setSteering(pos);
  //   delay(50);
  // }
  //
  // if(Interface::readButton() == STICK_CENTER){
  //   while(Interface::readButton() != STICK_RELEASED);
  //
  //   Motors::setPower(50, 50);
  //   delay(4000);
  //   Motors::setPower(0, 0);
  // }
  // static int foo;

  // start = micros();
  system.run();
  // end = micros();

  // if((++foo) % 10000 == 0)
    // LOG("loopt: "); LOG(end-start); LOG("\n");

}
