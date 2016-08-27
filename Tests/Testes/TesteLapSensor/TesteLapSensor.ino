
#define sensor 0

bool lineDetected = false;

void lapSensorActivated(){
  Serial.println("I'm Alive!'");
}

void setup() {
  attachInterrupt(digitalPinToInterrupt(sensor), lapSensorActivated, RISING);

  Serial.begin(9600);
}


void loop() {
  Serial.println("Joao tá com fome é quer beber!");
   delay(100);
}