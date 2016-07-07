#define encoder0PinA 2
#define encoder0PinB 3

#ifndef ENCODER_H
#define ENCODER_H

class Encoder{

public:
  static void init();
  static float getSpeed();
  static void doEncoderA;
  static void doEncoderB;
  unsigned int encoder0Pos = 0;
  unsigned int tmp_Pos = 1;
  float speed=0;
  int turn=0;
  boolean A_set;
  boolean B_set;
};

#endif
