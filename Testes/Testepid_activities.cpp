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

float error = 0;
float last_error = 0;
float motor_P = 0;
float motor_D = 0;
float motor_1 = 0;
float motor_2 = 0;
float steer = 0;
float steer_p = 0;
float steer_D = 0;
float curve = 0;
unsigned long last_time = 0;

void activityLineFollower_run(){
  static unsigned long start;
  static unsigned long lastStart;
  static unsigned long startedTime;
  
  static bool started;
  static int8_t lastSide;

  //Lap Sensor Interrupt
  int cross_counter = CROSS_COUNTER;
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), lapSensorActivated, RISING);

  if(Runner::invalidated){
    lastSide = 0;
    start = millis();
    started = false;
   // integral = 0;
   // realError = 0;
   // lastError = 0;

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(BLACK, WHITE);
    display.setCursor(20, 5);
    display.print("Line Follow");
    display.display();
    Runner::invalidated = false;

    Robot::doBeep(3, TIME_INTERVAL);
  };
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
  lastStart = micros();
  
  //
  //Start Line FOLLOWING LOOP
  //
  while(true){
    start = micros();
    //dt = start - lastStart;
   // lastStart = start;
    
    //
    // Stop if Button Pressed
    //
    if(Interface::readBtnState() == STICK_CENTER){
      Motors::setPower(0, 0);
      Motors::setSteering(0, true);
      Runner::exit();
      detachInterrupt(digitalPinToInterrupt(1)); 
      return;
    }

    //
    // Stop if Lap End
    //
    if(lineDetected && (millis() - last_time) > 50){       
      lineDetected = false;
      cross_counter--;
      last_time = millis();
      if(cross_counter <= 0){
        Motors::setPower(0, 0);
        Motors::setSteering(0, true);
        Runner::exit();
        detachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN));          
        return;
      } 
    }

    //
    // Follow Line controll
    //
    #define MINIMUM_SPEED         15
    #define BASE_SPEED            50
    
    //
    // MOTOR Constants
    //
    #define MOTOR_KP              0
    #define MOTOR_KD              0
    
    //
    // STEERING Constants
    //
    #define STEERING_KP           77.25    
    #define STEERING_KD           0
    #define STEERING_CURVE_START  0.45

    //
    // Read Sensor and get Error
    //
    LineReader::readValues();
    error = LineReader::getPosition();
    
    Serial.println(error);
    
  
    
    //
    // Determine if pass a Crossroad or leave line
    //
    if(isnan(error)){
      error = lastSide * 1;//2;
    }else{
      lastSide = (error > 0 ? 1 : (error < 0 ? -1 : 0));
    }

    //
    // Calculate Curve

    if(error > STEERING_CURVE_START)
      curve = error - STEERING_CURVE_START;
    else if(error < -STEERING_CURVE_START)
        curve = error + STEERING_CURVE_START;
    curve = fabs(error) > STEERING_CURVE_START ? (error - (fabs(error) / error) * STEERING_CURVE_START) : 0;
    curve = curve * STEERING_KP;

    //
    // Do Proportionals
    //
    motor_P = error * MOTOR_KP;
    steer_p = error * STEERING_KP;

    

    //
    // Do Diferentials
    //
    motor_D = (error - last_error) * MOTOR_KD;
    steer_D = (error - last_error) * STEERING_KD;

    //
    // Finish Derivative Process
    //
    last_error = error;

    //
    // Calculate motors PID
    //
    motor_1 = BASE_SPEED + motor_P  + motor_D; 
    motor_2 = BASE_SPEED - motor_P  - motor_D;

    //
    // Calculate Steer PID
    //
    steer = steer_p + steer_D + curve;

    //
    // Apply motors power
    //
    Motors::setPower(motor_1, motor_2);

    //
    // Apply steer power
    //
    Motors::setSteering(steer);
    

    //g[0] = K_PROPORTINAL + K_INTEGRAL/2 + K_DERIVATIVE;
    //g[1] = -K_PROPORTINAL + K_INTEGRAL/2 - 2*K_DERIVATIVE;
    //g[2] = K_DERIVATIVE;
    //error2 = error1;
    //error1 = error;

    

    // Moving average of error
    //realError = realError + (error - realError) * ERROR_APPROACH;
     //error = realError;

    // Integral
    //integral += dt * min(max(error, -STEERING_CURVE_START), STEERING_CURVE_START) * K_INTEGRAL;

    // Derivative
    //derivative = (error - lastError) * K_DERIVATIVE;
    //lastError = error;

    // curve = 0;
    // if(error > STEERING_CURVE_START)
    //   curve = error - STEERING_CURVE_START;
    // else if(error < -STEERING_CURVE_START)
    //   curve = error + STEERING_CURVE_START;
    
    // // if(Serial){
    //   Serial.print(error); 
    //   Serial.print(" "); 
    //   Serial.println(curve);
    // }
       
    //curve = fabs(error) > STEERING_CURVE_START ?
    //(error - (fabs(error) / error) * STEERING_CURVE_START) : 0;

    // speedDown = 1 - fmax(fmin(fabs(error) - STEERING_CURVE_START, 1.0), 0.0) * SPEEDOWN_MULT;
    // speedDown = speedDown < 0 ? 0 : speedDown;

//     m1 = BASE_SPEED + MOTOR_KP * error; //+ integral - derivative;
//     m2 = BASE_SPEED - MOTOR_KP * error; // - integral + derivative;

//     // Motors::setPower(
//     //   m1 * speedDown,
//     //   m2 * speedDown
//     // );
//     Motors::setPower(m1, m2);

//  //   Motors::setSteering(error * STEERING_NORMAL_MULT + curve * STEERING_CURVE_MULT);
//     Motors::setSteering(error * STEERING_KP);
//     //mv[1] = mv[0];
//     //mv[0] = mv[1] + error*g[0] + error1*g[1] + error2*g[2];
    //Motors::setSteering(mv[0]);
    //Serial.println(dt);
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
