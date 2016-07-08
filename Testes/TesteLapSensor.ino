
#define sensor TX
void setup() {
  pinMode(sensor, INPUT); 
  Serial.begin(9600);
}

void loop() {
  Serial.println(digitalRead(sensor));
  delay(100);

}