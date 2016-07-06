#include "_config.h"
#include "_types.h"

#ifndef MUX_H
#define MUX_H

class LineReader{
public:
  static void init();
  static void readValues();
  static int getValue(int index);
  static float getPosition();
};
#endif
