#include <SPI.h>
#include <Wire.h>
#include <Thread.h>
#include <Arduino.h>

#include "_config.h"
#include "_types.h"

#include "robot.h"
#include "runner.h"
#include "interface.h"
#include "activities.h"

//
// Shows activity menu and StatusBar
//
void actMenu_run();
Thread actMenu(actMenu_run, 50);
Activity menuActivity = { "Menu", &actMenu };


//
// Test menu thread
//
void actTest_run();
Thread actTest(actTest_run, 50);


//
// Activity configuration
//
Activity *runningActivity;
Activity menus[] = {
  // Test
  { "Line Follower",      &activityLineFollower},
  { "Line Reader",        &activityLineReader},
  { "Test Actuators",     &activityActuators},
  { "Steering Config",    &activityCalibrateSteering},
  // { "Calibrate",    &actTest},
  // { "Jhon",         &actTest}
};


// Initialize menus and refresh screen
bool Runner::invalidated = true;
void Runner::init(){
  // LOG("Runner init\n");

  Runner::setActivity(&menuActivity);
}


// ====================================
//       SETS THE CURRENT ACTIVIY
// ====================================
void Runner::setActivity(Activity *activity){
  // Remove last activity
  if(runningActivity)
    system.remove(runningActivity->thread);

  // Adds new one
  if(activity)
    system.add(activity->thread);

  runningActivity = activity;

  // Set flag of invalidated
  Runner::invalidated = true;

  // Debug
  // Serial.print("$ setActivity: ");
  // Serial.println(activity ? activity->title : "null");
}

// ====================================
//     SET MENU ACTIVITY AS CURRENT
// ====================================
void Runner::exit(){
  Robot::doBeep(2, 30);
  Runner::setActivity(&menuActivity);
}


// ====================================
//        ACTIVITY MENU THREAD
// ====================================
void actMenu_run(){
  static uint8_t _activities = sizeof(menus) / sizeof(menus[0]);
  static uint8_t _btnState = STICK_RELEASED;
  static uint8_t _currentSelection= 0;
  static int _updates = 0;

  // Invalidate screen if looped 10 times
  if(_updates++ > 10)
    Runner::invalidated = true;

  // Check for actions
  _btnState = Interface::readBtnState();
  if(_btnState == STICK_DOWN || _btnState == STICK_UP){
    uint8_t delta = _btnState == STICK_DOWN ? 1 : -1;
    _currentSelection = (_activities + _currentSelection + delta) % _activities;
    Robot::doBeep(1, 50);
    Runner::invalidated = true;
  }else if(_btnState == STICK_CENTER){
    Robot::doBeep(1, 50);
    // Serial.println("RUN ACTIVITY");
    Runner::setActivity(&menus[_currentSelection]);
    return;
  }


  if(Runner::invalidated){
    // Serial.println("# actMenu_render");
    // Clear screen
    display.clearDisplay();

    // Update screen
    display.setCursor(90, 4);
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.print(Robot::vbat);

    // Compute last/next activities
    int prevActivity = (_activities + _currentSelection - 1) % _activities;
    int nextActivity = (_activities + _currentSelection + 1) % _activities;

    // Render previous activity
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.print(menus[prevActivity].title);
    display.setCursor(0, 50);
    display.print(menus[nextActivity].title);

    // Render next activity
    display.setTextColor(BLACK, WHITE);
    display.setCursor(0, 30);
    display.print("> ");
    display.print(menus[_currentSelection].title);

    // Update screen
    display.display();

    // Set as non invalidated
    Runner::invalidated = false;
    _updates = 0;
  }

  // LineReader::readValues();
  // float pos = LineReader::getPosition();
  // Serial.print("Pos: ");
  // Serial.println(pos);
  // for(int i = 8; i < 16; i++){
  //   // Serial.print(i);
  //   // Serial.print(" ");
  //   Serial.print(LineReader::getValue(i));
  //   Serial.print("\t");
  // }
  // Serial.println();
}
