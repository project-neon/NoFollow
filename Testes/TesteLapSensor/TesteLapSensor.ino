
#define sensor 1
void setup() {
  pinMode(sensor, INPUT); 
  Serial.begin(9600);
}

void loop() {
  Serial.println(digitalRead(sensor));

}