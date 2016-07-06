// this class will contain the mais code that's called by the robot
//It will initiate the diferents robot's functions in the propper and logical order
// System Includes

// External Libraries
// #include <SPI.h>
#include <SPI.h>
#include <Wire.h>
#include <I2Cdev.h>
#include <Thread.h>
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
#include "interface.h"
#include "communication.h"
// #include "controller.h"

Adafruit_SSD1306 display(4);
#if (SSD1306_LCDHEIGHT != 64)
  #error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void setup(){

  // Initialize Robot state and I/O Pins
  Robot::init();

  // Initialize Attitude/Orientation module (initializes MPU6050 internally)
  // Attitude::init();

  // Initialize Interface Display/Buttons
  Interface::init();

  // Initialize Communication
  Communication::init();

  // Initalize Motors (object)
  Motors::init();

  // Initialize Motor controllers (with PID's)
  // Controller::init();

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Hello, world!");
  display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.println(3.141592);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.print("0x"); display.println(0xDEADBEEF, HEX);
  display.display();
}

// long start, end;
void loop(){
  // static int foo;

  // start = micros();
  system.run();
  // end = micros();

  // if((++foo) % 10000 == 0)
    // LOG("loopt: "); LOG(end-start); LOG("\n");

}
