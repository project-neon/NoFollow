#include <Servo.h>

Servo myservo1;
Servo myservo2;
 

void setup() {
  myservo1.attach(13);
  myservo2.attach(9);
}

void loop() {
  myservo1.write(90);
  myservo2.write(90);                  
  delay(15);
}
