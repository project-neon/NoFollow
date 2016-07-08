#include <Arduino.h>
#include <EEPROM.h>

#include "_config.h"
#include "_types.h"

#include "lineReader.h"

int values[16];
int SENSOR_MIN = 10;
int SENSOR_MAX = 100;

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
  pinMode(PIN_MUX_ENABLE, OUTPUT);
  digitalWrite(PIN_MUX_ENABLE, HIGH);

  // SENSOR_MIN = EEPROM.read(EEPROM_ADR_SENSOR_MIN);
  // SENSOR_MAX = EEPROM.read(EEPROM_ADR_SENSOR_MAX);

}

void LineReader::readValues(){
	for(uint8_t i = 8; i < 16; i++){
		for(int n = 0; n < 4; n++){
			digitalWrite(MUX_INS[n], i & (0x1 << n));
		}
		values[i] = analogRead(PIN_MUX_AN) / 4;
	}
}

int LineReader::getValue(int index){
	return values[index];
}

float LineReader::getPosition(){
  float weighted_sum = 0;
  float sum = 0;
  for(int i = 8; i < 16; i++){
    float value = -(float)getValue(i) / (SENSOR_MAX - SENSOR_MIN) + 1.0;
    value = fmin(fmax(0.0, value), 1.0);
    //  Serial.print(i);
    //  Serial.print(" : ");
    //  Serial.print(value);
    //  Serial.print("\t");
    weighted_sum = weighted_sum + value  * (i - 7);
    sum = sum + value;
  }
  // Serial.println();
  float position = -((weighted_sum / sum) - 4.5) / 3.5;
  return position;
}
