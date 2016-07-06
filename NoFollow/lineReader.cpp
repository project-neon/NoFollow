#include <Arduino.h>

#include "_config.h"
#include "_types.h"

#include "lineReader.h"

int values[16];

int MUX_INS[] = {
  PIN_MUX_S0,
  PIN_MUX_S1,
  PIN_MUX_S2,
  PIN_MUX_S3
};

void LineReader::init(){
  pinMode(PIN_MUX_AN, INPUT);
  pinMode(PIN_MUX_S0, OUTPUT);
  pinMode(PIN_MUX_S1, OUTPUT);
  pinMode(PIN_MUX_S2, OUTPUT);
  pinMode(PIN_MUX_S3, OUTPUT);
}

void LineReader::readValues(){
	for(uint8_t i = 8; i < 16; i++){
		for(int n = 0; i < 4; i++){
			digitalWrite(MUX_INS[n], i & (0x1 << n));
		}
		values[i] = analogRead(PIN_MUX_AN);
	}
}

int LineReader::getValue(int index){
	return values[index];
}
