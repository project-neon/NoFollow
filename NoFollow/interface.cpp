#include <SPI.h>
#include <Wire.h>
#include <Thread.h>
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "_config.h"
#include "_types.h"

#include "robot.h"
#include "runner.h"
#include "interface.h"


//
// Display
//
#if (SSD1306_LCDHEIGHT != 64)
  #error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
Adafruit_SSD1306 display(4);


//
// Thread: Serial Checke
//
void threadSerial_run();
Thread threadSerial(threadSerial_run, 5);


//
// STICK States
//
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
  system.add(&threadSerial);

  // Initialize Display
  display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDRESS);
  display.clearDisplay();


  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(7,20);
  display.println(PROJECT_NAME);
  display.setCursor(85,32);
  display.println(PROJECT_VERSION);
  display.display();
  delay(1000);

  // display.display();
  // display.setTextColor(WHITE);
  // display.setTextColor(BLACK, WHITE); // 'inverted' text
  // display.println(3.141592);
  // display.setTextSize(2);
  // display.setTextColor(WHITE);
  // display.print("0x"); display.println(0xDEADBEEF, HEX);
  // display.display();


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

uint8_t Interface::readButton(){
  static uint8_t lenght = sizeof(STICK) / sizeof(STICK[0]);
	uint8_t state = 0;
	uint8_t minError = 255;
	uint8_t val = analogRead(PIN_JOYSTICK) / 4;

	for(int i = 0; i < sizeof(STICK) / sizeof(STICK[0]); i++){
		int diff = abs(val - STICK[i]);

		if(diff < minError){
			minError = diff;
			state = i;
		}
	}

  return STICK[state];
}

// Filters actions with a minimum click time and a timeout
uint8_t Interface::readBtnState(){
  static uint8_t lastAction = STICK_RELEASED;
  static unsigned long lastTimestamp = 0;

  uint8_t btnVal = Interface::readButton();

  // Released
  if(btnVal == lastAction )
    return STICK_RELEASED;

  // Save state
  uint8_t action = lastAction;
  lastAction = btnVal;

  // Check if clicked for too long (Timeout)
  if(btnVal == STICK_RELEASED && millis() - lastTimestamp < 1000){
    return action;
  }

  // Save start pressing timestamp
  lastTimestamp = millis();
  return STICK_RELEASED;
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


// ====================================
//  ACTIVITY: Test
// ====================================
void actTest_run(){
  Serial.println("Running Test");
}
