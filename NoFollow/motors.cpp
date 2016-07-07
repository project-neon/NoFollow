#include <Servo.h>
#include <EEPROM.h>
#include <Arduino.h>

#include "_config.h"
#include "_types.h"

#include "robot.h"
#include "motors.h"

float Motors::avgSpeed = 0;
int8_t Motors::steeringPhase = 0;
int8_t Motors::steeringConverge = 0;

Servo ackermanLeft;
Servo ackermanRight;

// Initialize pins
void Motors::init(){
	// LOG("Motors::init\n");

  // Load Steering settigs from EEPROM
  Motors::steeringPhase = EEPROM.read(EEPROM_STEER_PHASE);
  Motors::steeringConverge = EEPROM.read(EEPROM_STEER_CONVERGE);

  // ackermanLeft.attach(PIN_ACKERMAN_LEFT);
  // ackermanLeft.write(90);

  // ackermanRight.attach(PIN_ACKERMAN_RIGHT);
  // ackermanRight.write(90);

	pinMode(PIN_M1_EN, OUTPUT);
	pinMode(PIN_M2_EN, OUTPUT);

	pinMode(PIN_M1_IN1, OUTPUT);
	pinMode(PIN_M2_IN1, OUTPUT);

	pinMode(PIN_M1_IN2, OUTPUT);
	pinMode(PIN_M2_IN2, OUTPUT);

  Motors::setPower(0, 0);
  Motors::setSteering(0, true);
}

// Sets the steering for the wheels
#define SERVO_MAX       140
void Motors::setSteering(float steer, bool activate){
  static bool activated;

  if(activate && !activated){
    ackermanLeft.attach(PIN_ACKERMAN_LEFT);
    ackermanRight.attach(PIN_ACKERMAN_RIGHT);
    activated = true;
  }else if(!activate && activated){
    ackermanLeft.detach();
    ackermanRight.detach();
    activated = false;
  }
  // Calculate Limited Steering (Goes from -100 to 100)
  steer = -min(max(steer, -100.0), 100.0);

  // Applies Ackerman
  float left  = (steer > 0 ?  steer : 0) * 0.3;
  float right = (steer < 0 ? -steer : 0) * 0.3;

  left  = (  left + steer) * (SERVO_MAX / 200.0) + 90 + Motors::steeringPhase;
  right = (-right + steer) * (SERVO_MAX / 200.0) + 90 + Motors::steeringPhase;

  // Applies Phase
  steer = steer;
  // Serial.println(steer);

  ackermanLeft.write(  left + Motors::steeringConverge);
  ackermanRight.write(right - Motors::steeringConverge);
}

// Set power of both DC motors
void Motors::setPower(float m1, float m2){

	// Motors::avgSpeed = (m1 + m2) / 2.0;

	// Limit Powers
	m1 = min(max(m1, -100), 100);
	m2 = min(max(m2, -100), 100);

	// Map powers
	int powerOutA = m1 * (MOTOR_ABS_MAX * (1/100.0));
	int powerOutB = m2 * (MOTOR_ABS_MAX * (1/100.0));

	// Set power
	analogWrite(PIN_M1_EN, abs(powerOutA));
	analogWrite(PIN_M2_EN, abs(powerOutB));

	// Set Directions
	digitalWrite(PIN_M1_IN1, powerOutA >  0 ? HIGH : LOW);
	digitalWrite(PIN_M1_IN2, powerOutA >= 0 ? LOW : HIGH);

	digitalWrite(PIN_M2_IN1, powerOutB >  0 ? HIGH : LOW);
	digitalWrite(PIN_M2_IN2, powerOutB >= 0 ? LOW : HIGH);

}

void Motors::stop(){
	// Stop motor 1
	digitalWrite(PIN_M1_EN, LOW);
	digitalWrite(PIN_M1_IN1, LOW);
  digitalWrite(PIN_M1_IN2, LOW);

  // Stop motor 2
  digitalWrite(PIN_M2_EN, LOW);
  digitalWrite(PIN_M2_IN1, LOW);
  digitalWrite(PIN_M2_IN2, LOW);
}
