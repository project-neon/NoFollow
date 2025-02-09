#include <Thread.h>
#include <EEPROM.h>
#include <Arduino.h>

#include "_config.h"
#include "_types.h"

#include "robot.h"
#include "runner.h"
#include "motors.h"
#include "interface.h"
#include "lineReader.h"

bool lineDetected = false;

void lapSensorActivated(){
  lineDetected = true;
}

// ====================================
// ACTIVITY: Line Follower
// ====================================
void activityLineFollower_run();
Thread activityLineFollower(activityLineFollower_run, 3);

#define TIME_INTERVAL     600
#define TIME_TO_START     TIME_INTERVAL * 3
//long spees = 0;
float error = 0;
//float error1 = 0;
//float error2 = 0;
//float mv[] = {0, 0};
int SECCONDS = 23;

void activityLineFollower_run(){
  static unsigned long start;
  static unsigned long lastStart;
  static unsigned long startedTime;
  
  static bool started;
  static int8_t lastSide;
  static double integral;
  static double realError;
  static float lastError;
  unsigned long last_time = 0;
  static float steer;
  unsigned int side;

  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), lapSensorActivated, RISING);
  
  if(Runner::invalidated){
    lastSide = 0;
    start = millis();
    started = false;
    integral = 0;
    realError = 0;
    lastError = 0;
    lineDetected = false;
    steer = 0;
    side = 0;

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(BLACK, WHITE);
    display.setCursor(20, 5);
    display.print("Line Follow");
    display.display();
    Runner::invalidated = false;

    Robot::doBeep(3, TIME_INTERVAL);
  };
  int btn = Interface::readBtnState();
  if(!started){// && millis() - start < TIME_TO_START){
    display.setTextSize(3);
    display.setTextColor(WHITE, BLACK);
    display.setCursor(45, 30);
    //if(millis() - start > TIME_TO_START - 100){
    if(btn == STICK_CENTER){  
      display.print("GO!");

      // Set started flag
      started = true;
      startedTime = millis();
    }else{
      if(btn == STICK_DOWN)
        SECCONDS--;
      if(btn == STICK_UP)
        SECCONDS++;
      display.print(SECCONDS);
    }
    display.display();
    return;
  }

  //Lap Sensor Interrupt
  int cross_counter = CROSS_COUNTER;
  //float g[3];
  float derivative;
  float curve;
  float speedDown;
  float m1;
  float m2;
  int dt = 0;
  lastStart = micros();
  //unsigned long start;
  while(true){
    start = micros();
    dt = start - lastStart;
    lastStart = start;
    
    if(Interface::readBtnState() == STICK_LEFT){
      Motors::setPower(0, 0);
      Motors::setSteering(0, true);
      Runner::exit();
      detachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN)); 
      return;
    }

    if(curve > 0.1){
      side = 1;
    }else if(curve < 0.1){
      side = -1;
    }else{
      side = 0;
    }

    //if(lineDetected && (millis() - last_time) > 100 && (millis() - startedTime) > SECCONDS * 1000 && side < 0.1){       
      if((millis() - startedTime) > SECCONDS * 1000) {
        last_time = millis();
      // if(cross_counter <= 0){
        Motors::setPower(0, 0);
        Motors::setSteering(0, true);
        Runner::exit();
        detachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN));          
        return;
      //} 
    }

    lineDetected = false;

    //
    // Follow Line controll
    //
    #define MINIMUM_SPEED         15
    #define BASE_SPEED            30  
    #define CURVE_DIFERENTIAL     10
    #define ERROR_APPROACH        0
    #define DIFERENTIAL_START     0.75

    #define K_INTEGRAL            0.0000000
    #define K_DERIVATIVE          150 
    #define K_PROPORTINAL         0  //ADD BY VITOR

    // Definições para Steering
    #define STEERING_NORMAL_MULT  15.75
    #define STEERING_CURVE_MULT   60 //60
    #define STEERING_CURVE_START  0.48 //0.55
    #define SPEEDOWN_MULT         0.0    

    //GET Error
    LineReader::readValues();
    error = LineReader::getPosition();
    //Set Last Direction Detected
    if(isnan(error)){
      error = lastSide * 2;
    }else{
      lastSide = (error > 0 ? 1 : (error < 0 ? -1 : 0));
    }
    //Determine curve Side
    curve = 0;
    if(error > STEERING_CURVE_START)
      curve = error - STEERING_CURVE_START;
    else if(error < -STEERING_CURVE_START)
      curve = error + STEERING_CURVE_START;
      
      

    //Determine reduction to be applied in motors
    speedDown = 1 - fmax(fmin(fabs(error) - STEERING_CURVE_START, 1.0), 0.0) * SPEEDOWN_MULT;
    speedDown = speedDown < 0 ? 0 : speedDown;

    
    // Derivative
    derivative = (error - lastError) * K_DERIVATIVE;
    lastError = error;

    //Calculate motors speed
    m1 = BASE_SPEED + (abs(error) > DIFERENTIAL_START ? error * CURVE_DIFERENTIAL : 0);// + curve * CURVE_DIFERENTIAL; //+ integral - derivative;
    m2 = BASE_SPEED - (abs(error) > DIFERENTIAL_START ? error * CURVE_DIFERENTIAL : 0);//curve * CURVE_DIFERENTIAL; // - integral + derivative;
    //Apply motors power with reduciton
    Motors::setPower(
      m1 * speedDown,
      m2 * speedDown
    );
    //Apply Steering 
    steer = error * STEERING_NORMAL_MULT + curve * STEERING_CURVE_MULT + derivative;// ;
    Motors::setSteering(steer);

    // Moving average of error
    //realError = realError + (error - realError) * ERROR_APPROACH;
    // error = realError;

    // Integral
    //integral += dt * min(max(error, -STEERING_CURVE_START), STEERING_CURVE_START) * K_INTEGRAL;

    //curve = fabs(error) > STEERING_CURVE_START ?
    //(error - (fabs(error) / error) * STEERING_CURVE_START) : 0;
  }
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

  if(btnState == STICK_RELEASED){
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
    display.print("  <  ");
  }else if(btnState == STICK_RIGHT){
    Motors::setPower(0, 0);
    Motors::setSteering(100);
    display.print("  >  ");
  }

  display.display();
}



// ====================================
// ACTIVITY: Calibrate Steering
// ====================================
void activityCalibrateSteering_run();
Thread activityCalibrateSteering(activityCalibrateSteering_run, 5);

void activityCalibrateSteering_run(){
  if(Runner::invalidated){
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(BLACK, WHITE);
    display.setCursor(15, 5);
    display.print("Calibrate Steering");

    Runner::invalidated = false;
  }

  int btnState = Interface::readBtnState();
  int btnValue = Interface::readButton();

  if(btnState == STICK_CENTER){
    // Save to eeprom
    EEPROM.write(EEPROM_STEER_PHASE, Motors::steeringPhase);
    EEPROM.write(EEPROM_STEER_CONVERGE, Motors::steeringConverge);

    // Reset actuators
    Motors::setPower(0, 0);
    Motors::setSteering(0, true);
    Runner::exit();
    return;
  }else if(btnValue == STICK_RIGHT){
    if(Motors::steeringPhase < 40)
      Motors::steeringPhase++;
  }else if(btnValue == STICK_LEFT){
    if(Motors::steeringPhase > -40)
      Motors::steeringPhase--;
  }else if(btnValue == STICK_DOWN){
    if(Motors::steeringConverge > -40)
      Motors::steeringConverge--;
  }else if(btnValue == STICK_UP){
    if(Motors::steeringConverge < 40)
      Motors::steeringConverge++;
  }

  if(btnState != STICK_CENTER && btnState != STICK_RELEASED){
    Robot::doBeep(1, 0);
  }


  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(0, 30);

  display.print("Converge:");
  display.print(Motors::steeringConverge);
  display.println("   ");

  display.print("Converge:");
  display.print(Motors::steeringPhase);
  display.println("   ");

  display.display();

  // Update steering
  Motors::setSteering(0);

}



// ====================================
// ACTIVITY: LINE READER DEBUG
// ====================================
void activityLineReader_run();
//Thread activityLineReader(activityLineReader_run, 10);

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
