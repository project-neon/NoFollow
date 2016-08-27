#include <Arduino.h>

#include "_config.h"
#include "_types.h"

#include "encoder.h"

void Encoder::Init(){

  pinMode(encoder0PinA, INPUT); 
  pinMode(encoder0PinB, INPUT); 

// encoder pin on interrupt 0 (pin 2)
  attachInterrupt(0, doEncoderA, CHANGE);

// encoder pin on interrupt 1 (pin 3)
  attachInterrupt(1, doEncoderB, CHANGE);

  Serial.begin (9600);
}

/*
void loop(){ 
//Check each second for change in position
  if (tmp_Pos != encoder0Pos) {
    Serial.print("Index:"); Serial.print(encoder0Pos, DEC);
    Serial.println();
	turn = encoder0Pos/360;
	speed = (encoder0Pos/360.0) * 3.141592654 * 0.030;
	Serial.print("Turns of the motor:"); Serial.print(turn, DEC); 
	Serial.println("  RPS");
	Serial.print("Speed of the motor:"); Serial.print(turn, DEC); 
	Serial.println("  m/s");
    tmp_Pos = encoder0Pos;
  }
  delay(1000);
}*/

float Encoder:: getSpeed(encoder0Pos){
	return ((encoder0Pos/360.0) * 3.141592654 * 0.030);
}


// Interrupt on A changing state
void Encoder::doEncoderA(){

  // Low to High transition?
  if (digitalRead(encoder0PinA) == HIGH) { 
    A_set = true;
    if (!B_set) {
      encoder0Pos = encoder0Pos + 1;      
    }        
  }

  // High-to-low transition?
  if (digitalRead(encoder0PinA) == LOW) {
    A_set = false;
  }

}

// Interrupt on B changing state
void Encoder::doEncoderB(){

  // Low-to-high transition?
  if (digitalRead(encoder0PinB) == HIGH) {   
    B_set = true;
    if (!A_set) {
      encoder0Pos = encoder0Pos - 1;
    }
  }

  // High-to-low transition?
  if (digitalRead(encoder0PinB) == LOW) {
    B_set = false;
  }
}

