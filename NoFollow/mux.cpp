#include "_config.h"
#include "_types.h"

#ifndef MUX_H
#define MUX_H

class Mux{
int values [16];
	static S[4] = {PIN_MUX_S0, PIN_MUX_S1, PIN_MUX_S2, PIN_MUX_S3};
	
	public: readValues(){
		for(uint8_t i = 0; i<16; i++){
			for(int n = 0; i<4; i++){
				digitalWrite(S[n], i&(0x1<<n));
			}
			values [i] = analogRead(PIN_MUX_AN);
		}
	}
	public: getValue(int i){
		return values[i];
	}
	public: getValues(){
		return values;
	}
};

#endif