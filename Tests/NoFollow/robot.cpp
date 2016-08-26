#include <Arduino.h>
#include <Thread.h>

#include "_config.h"
#include "_types.h"

#include "imu.h"
#include "robot.h"
#include "motors.h"

//
// Main Thread Controller
//
ThreadController system;

//
// Thread: Beeper
//
void threadBeeper_run();
Thread threadBeeper(threadBeeper_run, 5);

//
// Thread: Battery Checker
//
//void threadBatteryChecker_run();
//Thread threadBatteryChecker(threadBatteryChecker_run, 3000);

//
// Thread: LED thread
//
//void threadWatchdog_run();
//Thread threadWatchdog(threadWatchdog_run, 500);

//
// Thread: IMU
//
void imu_run();
// IMUThread imuSensor;


// ====================================
//           INITIALIZERS
// ====================================
void Robot::init(){

  // Initialize Serial and Wait to be ok
  Serial.begin(SERIAL_SPEED);
  // while(!Serial && millis() < 2000);
  delay(50);

  // Debug init
  LOG("Robot::init\n");

  // Log current version
  LOG("\n===== "); LOG(PROJECT_NAME); LOG(" =====\n");
  LOG(PROJECT_VERSION); LOG("\n\n");

  // Configure I2C
  Wire.begin();
  TWBR = ((16000000L / 400000L) - 16) / 2;

  // Configure LED
  // DDRE |= (1<<2);

  // Configure Buzzer
  pinMode(PIN_BUZZER, OUTPUT);

  // Configure Battery meter
  pinMode(PIN_VBAT, INPUT);

  // Setup Beeper Thread
  Robot::setBeep(BEEP_NONE);

  // Initialize IMU
  // imuSensor = new IMUThread();
  // imuSensor.init();
  // imuSensor.setInterval(9);
  pinMode(1, INPUT);

  // Add threads to system
  // system.add(&imuSensor);
  //system.add(&threadBeeper);
  //system.add(&threadWatchdog);
  //system.add(&threadBatteryChecker);
};


// ====================================
//            ROBOT STATE
// ====================================
RobotState Robot::state = IDDLE;

void Robot::setState(RobotState _state){
  state = _state;

  if(state == IDDLE)
    Motors::stop();
};


// ====================================
//       BEEP and Voltage states
// ====================================
float Robot::vbat = 0;
int Robot::beepTimes = 2;
int Robot::beepInterval = 50;
BeepState Robot::beepState = BEEP_NONE;

void Robot::setBeep(BeepState state){
  // Skip if already in that state
  if(Robot::beepState == state)
    return;

  Robot::beepState = state;
  threadBeeper.enabled = true;

  LOG("Beep State: "); LOG(state); LOG("\n");
}

void Robot::doBeep(int _times, int interval){
  Robot::beepTimes = _times;
  Robot::beepInterval = interval;

  // Make sure Thread will run
  threadBeeper.enabled = true;

  LOG("Beep times: "); LOG(_times); LOG("\n");
}

void threadBeeper_run(){
  static bool isBeeping = false;
  static bool lastIsBeeping = false;

  // if(Robot::alarm != NONE){
    // High Priority to alarms
    // isBeeping = !isBeeping;
    // threadBeeper.setInterval(isBeeping ? 20 : 80);
  // }else
  if(Robot::beepTimes > 0){
    // Decrease beeps at each beep
    if(!isBeeping)
      Robot::beepTimes--;

    isBeeping = !isBeeping;

    threadBeeper.setInterval(isBeeping ? 30 : Robot::beepInterval);
  }else if(Robot::beepState == BEEP_NONE){
    isBeeping = false;
    threadBeeper.enabled = false;
    threadBeeper.setInterval(0);
  }else if(Robot::beepState == WARN){
    isBeeping = !isBeeping;

    threadBeeper.setInterval(isBeeping ? 30 : 1000);
  }else if(Robot::beepState == ALARM){
    isBeeping = !isBeeping;

    threadBeeper.setInterval(isBeeping ? 30 : 200);
  }

  if(lastIsBeeping != isBeeping){
    digitalWrite(PIN_BUZZER, isBeeping);
    lastIsBeeping = isBeeping;
  }
}


// ====================================
//           BATTERY CHECKER
// ====================================
//
// Checks battery voltage
//
void threadBatteryChecker_run(){
  static int alerts = 0;

  long analogVal = analogRead(PIN_VBAT);
  Robot::vbat = VBAT_VOLTAGE(analogVal);
  // LOG("VBAT: "); LOG(Robot::vbat); LOG(" ["); LOG(analogVal); LOG("]\n");

  if(Robot::vbat < VBAT_ALARMED){
    if(++alerts > 3){
      Robot::setBeep(ALARM);
    }
  }else if(Robot::vbat < VBAT_WARNED){
    if(alerts <= 0)
      Robot::setBeep(WARN);
  }else{
    // Reset filter
    alerts = 0;
    Robot::setBeep(BEEP_NONE);
  }
}


// ====================================
//        WATCHDOG/LED Thread
// ====================================
//
// Watches role robot and checks for activity. Also, toggles led
//
void threadWatchdog_run(){
  static bool ledState = false;

  // Toggle state
  ledState = !ledState;

  // Check for ALARM
  // if(Robot::alarm != NONE)
  // ledState = true;

  // Set led power to ledState state
  // ledState ? (PORTE |=  (1<<2)) : (PORTE &= ~(1<<2));

  // Set timeout acordingly to Robot's state
  //threadWatchdog.setInterval(Robot::state == IDDLE ? 500 : 100);
}


// ====================================
//         ATTITUDE/ORIENTATION
// ====================================
Quaternion Robot::q = Quaternion();
VectorFloat Robot::gravity = VectorFloat();
float Robot::ypr[3] = {0,0,0};
