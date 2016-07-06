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
static int STICK[5] = {STICK_RELEASED, STICK_UP, STICK_LEFT, STICK_DOWN, STICK_RIGHT};
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
int Interface::readValue(int val){
	int state =-1;
	int minError = 256;
	val = val/4;
	int lenght =sizeof(SITCK)/sizeof(STICK[0]
	for(int i = 0; i < sizeof(SITCK)/sizeof(STICK[0]); i++){
		int diff = val > STICK[i] ? val - STICK[i] : STICK[i] - val;
		if(diff < minError){
			minError = diff;
			state = i;
		}
	return STICK[state];
	}
}
