#include <stdint.h>
#include <math.h>

#ifndef CONFIG_H
#define CONFIG_H

//
// PINS
//TODO: AJUST THE RIGHT PINS
#define PIN_M1_EN							1
#define PIN_M1_IN1							2
#define PIN_M1_IN2							3

#define PIN_M2_EN							4
#define PIN_M2_IN1							5
#define PIN_M2_IN2							6

#define PIN_M3_EN							7
#define PIN_M3_IN1							8
#define PIN_M3_IN2							9

//
// Project
//
#define PROJECT_NAME						"LineFollowFirmware"
#define PROJECT_VERSION 					"v0.01"


//
// Serial DEBUG
//
#define SERIAL_SPEED 						9600
#define LOG 								Serial.print


//
// Motors config (H-Bridge)
//


//
// VBat Reader (battery status)
//


//
// Bluetooth Configs (Type here)
//


#endif
