#include <SPI.h>
#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "_config.h"
#include "_types.h"

#include "robot.h"
#include "interface.h"

// Adafruit_SSD1306 display();

// Initialize pins
void Interface::init(){
  LOG("Interface::init\n");

  // display.begin(SSD1306_SWITCHCAPVCC, 0x3D);
  // display.display();
  //
  // // Clear the buffer.
  // display.clearDisplay();
  //
  // // draw a single pixel
  // display.drawPixel(10, 10, WHITE);
  // display.display();
}
