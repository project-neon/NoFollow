#define stick A4
int sensorValue = 0;
int outputValue = 0;
void setup() {
  Serial.begin(9600);
  pinMode(stick, INPUT_PULLUP);
}

void loop() {
  sensorValue = analogRead(stick);
  outputValue = map(sensorValue, 0, 1023, 0, 255);
  Serial.print("sensor = ");
  Serial.print(sensorValue);
  Serial.print("\tHex= ");
  Serial.println(outputValue);
  delay(100);
}
