/*This class will be responsible to parse the Bluetooth communication to and from the robot setting the Values
(like PID and sensor sensitivity) and providing the feedback*/

#include "_config.h"
#include "_types.h"

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

class Communication{
public:


	// ====================================
	//           INITIALIZATION
	// ====================================

	static void init();


	// ====================================
	//           MESSAGE PARSING
	// ====================================

	static bool handleMessage(uint8_t message[], uint8_t len);
	// Parse the message and take action
};



#endif
