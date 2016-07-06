#include <Arduino.h>
#include <Thread.h>

#include "_config.h"
#include "_types.h"

#include "robot.h"
#include "communication.h"

//
// Thread: Serial Checke
//
void threadSerial_run();
Thread threadSerial(threadSerial_run, 5);


void Communication::init(){
  // Debug init
  LOG("Communication::init\n");

  // Add threads to system
  system.add(&threadSerial);
}

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
