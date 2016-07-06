#include <SPI.h>
#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "_config.h"
#include "_types.h"

#include "robot.h"
#include "interface.h"

//
// Thread: Serial Checke
//
void threadSerial_run();
Thread threadSerial(threadSerial_run, 5);

// Adafruit_SSD1306 display();
static int STICK[] = {
  STICK_RELEASED,
  STICK_UP,
  STICK_LEFT,
  STICK_DOWN,
  STICK_RIGHT,
  STICK_CENTER
};

// Initialize pins
void Interface::init(){
	LOG("Interface::init\n");

  // Initialize Joystick
  pinMode(PIN_JOYSTICK, INPUT_PULLUP);

  // Initialize Serial and add debug thread to system
  Serial.begin(115200);
  system.add(&threadSerial);

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

int Interface::readButton(){
	int state = 0;
	int minError = 256;
  int lenght = sizeof(STICK) / sizeof(STICK[0]);
	int val = analogRead(PIN_JOYSTICK) / 4;

	for(int i = 0; i < sizeof(STICK) / sizeof(STICK[0]); i++){
		int diff = abs(val - STICK[i]);

		if(diff < minError){
			minError = diff;
			state = i;
		}
	}

  return STICK[state];
}


// ====================================
//   HANDLES USB SERIAL COMMUNICATION
// ====================================
void threadSerial_run(){
  if(!Serial.available())
    return;

  char got = Serial.read();

  LOG("got:"); LOG(got); LOG("\n");

  // if(got == '1')
  //   Motors::setPower(50, 0);
  //
  // else if(got == '2')
  //   Motors::setPower(-50, 0);
  //
  // else if(got == '3')
  //   Motors::setPower(0, 50);
  //
  // else if(got == '4')
  //   Motors::setPower(0, -50);

  // else if(got == '0')
  //   Motors::stop();

  if(got == 'b'){
    LOG("bat: "); LOG(Robot::vbat); LOG("v\n");

  }else if(got == 'h'){
    // LOG("\n---- help ----\n");
    // LOG("0: Stop motors\n");
    // LOG("b: Get bat. voltage\n");
    // LOG("i: robot ID\n");
  }
}
