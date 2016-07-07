#include <Thread.h>
#include <Arduino.h>

#include "_config.h"
#include "_types.h"

#include "robot.h"
#include "runner.h"
#include "motors.h"
#include "interface.h"
#include "lineReader.h"

// ====================================
// ACTIVITY: Line Follower
// ====================================
void activityLineFollower_run();
Thread activityLineFollower(activityLineFollower_run, 5);

#define TIME_INTERVAL     600
#define TIME_TO_START     TIME_INTERVAL * 3
void activityLineFollower_run(){
  static unsigned long start;
  static bool started;

  if(Runner::invalidated){
    start = millis();
    started = false;
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(BLACK, WHITE);
    display.setCursor(20, 5);
    display.print("Line Follow");
    display.display();
    Runner::invalidated = false;

    Robot::doBeep(3, TIME_INTERVAL);
  };

  if(Interface::readBtnState() != STICK_RELEASED){
    Motors::setPower(0, 0);
    Motors::setSteering(0, false);
    Runner::exit();
    return;
  }

  if(!started && millis() - start < TIME_TO_START){
    display.setTextSize(3);
    display.setTextColor(WHITE, BLACK);
    display.setCursor(45, 30);
    if(millis() - start > TIME_TO_START - 100){
      display.print("GO!");

      // Set started flag
      started = true;
    }else{
      display.print(3 - (millis() - start) / TIME_INTERVAL);
    }
    display.display();
    return;
  }

  //
  // Follow Line controll
  //
  LineReader::readValues();
  float error = LineReader::getPosition();

  float m1 = 0.6 - abs(error) * 0.0 - error * 0.0;
  float m2 = 0.6 - abs(error) * 0.0 + error * 0.0;

  Motors::setPower(m1, m2);
  Motors::setSteering(-error);
}


// ====================================
// ACTIVITY: Test Motors/Steering
// ====================================
void activityActuators_run();
Thread activityActuators(activityActuators_run, 5);

void activityActuators_run(){
  if(Runner::invalidated){
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(BLACK, WHITE);
    display.setCursor(15, 5);
    display.print("Test Actuators");

    Runner::invalidated = false;
  }

  if(Interface::readBtnState() == STICK_CENTER){
    Motors::setPower(0, 0);
    Motors::setSteering(0, false);
    Runner::exit();
    return;
  }

  int btnState = Interface::readButton();

  display.setTextSize(2);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(25, 30);

  if(btnState == STICK_CENTER){
    Motors::setPower(0, 0);
    Motors::setSteering(0);
    display.print(" ( ) ");
  }else if(btnState == STICK_DOWN){
    Motors::setPower(-100, -100);
    Motors::setSteering(0);
    display.print("  V  ");
  }else if(btnState == STICK_UP){
    Motors::setPower(100, 100);
    Motors::setSteering(0);
    display.print("  ^  ");
  }else if(btnState == STICK_LEFT){
    Motors::setPower(0, 0);
    Motors::setSteering(-100);
    display.print("  >  ");
  }else if(btnState == STICK_RIGHT){
    Motors::setPower(0, 0);
    Motors::setSteering(100);
    display.print("  <  ");
  }

  display.display();
}



// ====================================
// ACTIVITY: LINE READER DEBUG
// ====================================
void activityLineReader_run();
Thread activityLineReader(activityLineReader_run, 10);

void activityLineReader_run(){
  // Serial.println("activityLineReader_run");
  if(Runner::invalidated){
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(5, 5);
    display.print("Line Read");
    Runner::invalidated = false;
  }


  // Read Value
  LineReader::readValues();
  int value = LineReader::getPosition() * 50;

  display.setTextSize(1);
  // display.setTextColor(WHITE, BLACK);
  // display.setCursor(53, 30);
  // display.print("     ");
  display.setTextColor(WHITE, BLACK);
  display.setCursor(53, 30);
  display.print(value);

  display.fillRect(5, 40, 100, 20, WHITE);
  display.drawFastVLine(5 + 50 + value, 40, 20, BLACK);
  display.display();

  if(Interface::readBtnState() == STICK_LEFT)
    Runner::exit();
}
