#include <stdint.h>
#include <math.h>

#ifndef CONFIG_H
#define CONFIG_H


//
// Project
//
#define PROJECT_NAME            "LineFollowFirmware"
#define PROJECT_VERSION         "v1.00"


//
// Interface (Buzzer/Leds)
//
#define PIN_BUZZER              5
#define PIN_JOYSTICK            A3


//
// Serial DEBUG
//
#define SERIAL_SPEED 						115200
#define LOG 								    Serial.print


//
// Mux Settings
//
#define PIN_MUX_AN              A5
#define PIN_MUX_S0              6
#define PIN_MUX_S1              12
#define PIN_MUX_S2              7
#define PIN_MUX_S3              4

//
// Stick Settings
//
#define STICK_RELEASED          0
#define STICK_UP                64
#define STICK_LEFT              127
#define STICK_DOWN              192
#define STICK_RIGHT             255


//
// I2C Config (IMU/Display
//
#define IMU_INTERRUPT_CHANNEL   0



//
// Motors config (H-Bridge)
//
#define MOTOR_ABS_MAX           200
#define PIN_M1_EN               11
#define PIN_M1_IN1              15
#define PIN_M1_IN2              16

#define PIN_M2_EN               10
#define PIN_M2_IN1              14
#define PIN_M2_IN2              8


//
// VBat Reader (battery status)
//
#define VBAT_ALARMED           7.10
#define VBAT_WARNED            7.40

#define PIN_VBAT                A4
#define VBAT_VOLTAGE(adc)       adc
// ((adc - 40.3) / 88.3)


//
// Bluetooth Configs (Type here)
//
#define Bluetooth               Serial


#endif
