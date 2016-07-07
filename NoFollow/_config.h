#include <stdint.h>
#include <math.h>

#ifndef CONFIG_H
#define CONFIG_H


//
// Project
//
#define PROJECT_NAME            F("LineFollowFirmware")
#define PROJECT_VERSION         F("v1.00")


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
#define PIN_MUX_ENABLE          16
#define PIN_MUX_AN              A5
#define PIN_MUX_S0              6
#define PIN_MUX_S1              A1
#define PIN_MUX_S2              7
#define PIN_MUX_S3              4

//
// Stick Settings
//
#define STICK_RELEASED          254
#define STICK_UP                17
#define STICK_LEFT              3
#define STICK_DOWN              42
#define STICK_RIGHT             30
#define STICK_CENTER            52

//
// EEPROM Adresses
//
#define EEPROM_ADR_SENSOR_MIN   0x01
#define EEPROM_ADR_SENSOR_MAX   0x02


//
// I2C Config (IMU/Display
//
#define DISPLAY_ADDRESS         0x3C
#define IMU_INTERRUPT_CHANNEL   0

//
// Motors config (H-Bridge)
//
#define MOTOR_ABS_MAX           255
#define PIN_M1_EN               11
#define PIN_M1_IN1              MOSI
#define PIN_M1_IN2              SCK

#define PIN_M2_EN               10
#define PIN_M2_IN1              8
#define PIN_M2_IN2              MISO

//
// Servos for Ackerman
//
#define PIN_ACKERMAN_LEFT        13
#define PIN_ACKERMAN_RIGHT       9

//
// VBat Reader (battery status)
//
#define VBAT_ALARMED           7.10
#define VBAT_WARNED            7.40

#define PIN_VBAT                A4
#define VBAT_VOLTAGE(adc)       (adc - 534) * (8.23 - 6.47) / (662-534) + 6.47
// ((adc - 40.3) / 88.3)


//
// Bluetooth Configs (Type here)
//
#define Bluetooth               Serial


#endif
