//this class will have the motors configuration, monitoration and control
#include "_config.h"
#include "_types.h"

#ifndef INTERFACE_H
#define INTERFACE_H

class Interface{
public:
  static void init();
  static int readButton(int value);
};

#endif
