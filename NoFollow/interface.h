//this class will have the motors configuration, monitoration and control
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "_config.h"
#include "_types.h"


#ifndef INTERFACE_H
#define INTERFACE_H

class Interface{
public:
  static void init();
  static uint8_t readButton();
  static uint8_t readBtnState();
};

extern Adafruit_SSD1306 display;

#endif
