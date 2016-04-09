#include <Arduino.h>

#include "_config.h"
#include "_types.h"

#include "robot.h"
#include "motors.h"

float Motors::avgSpeed = 0;

// Initialize pins
void Motors::init(){
	LOG("Motors::init\n");

	pinMode(PIN_M1_EN, OUTPUT);
	pinMode(PIN_M2_EN, OUTPUT);
    pinMode(PIN_M3_EN, OUTPUT);
	
	pinMode(PIN_M1_IN1, OUTPUT);
	pinMode(PIN_M2_IN1, OUTPUT);
    pinMode(PIN_M3_IN1, OUTPUT);

	pinMode(PIN_M1_IN2, OUTPUT);
	pinMode(PIN_M2_IN2, OUTPUT);
    pinMode(PIN_M3_IN2, OUTPUT);
}

// Set power of both DC motors
void Motors::setPower(float m1, float m2){

	Motors::avgSpeed = (m1 + m2) / 2.0;

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
	digitalWrite(PIN_M1_IN1, powerOutA > 0 ? HIGH : LOW);
	digitalWrite(PIN_M1_IN2, powerOutA > 0 ? LOW : HIGH);

	digitalWrite(PIN_M2_IN1, powerOutB > 0 ? HIGH : LOW);
	digitalWrite(PIN_M2_IN2, powerOutB > 0 ? LOW : HIGH);
	
}

void Motors::stop(){
	// Set power (0)
	digitalWrite(PIN_M1_EN, LOW);
	digitalWrite(PIN_M2_EN, LOW);
    digitalWrite(PIN_M3_EN, LOW);

	// Set both DIRS to 0
	digitalWrite(PIN_M1_IN1, LOW);
	digitalWrite(PIN_M2_IN1, LOW);
    digitalWrite(PIN_M3_IN1, LOW);

	digitalWrite(PIN_M1_IN2, LOW);
	digitalWrite(PIN_M2_IN2, LOW);
    digitalWrite(PIN_M3_IN2, LOW);
}