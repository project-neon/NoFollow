#define PIN_MUX_S0 A1
#define PIN_MUX_S1 12
#define PIN_MUX_S2 7
#define PIN_MUX_S3 4
#define PIN_MUX_AN A5
#define PIN_MUX_ENABLE 16
int values[16];
int SENSOR_MIN = 100;
int SENSOR_MAX = 500;
int ini = 8;
int fim = 16;
static int S[] = {PIN_MUX_S0, PIN_MUX_S1, PIN_MUX_S2, PIN_MUX_S3};
  
void readValues(){
    for(uint8_t i = ini; i < fim; i++){
      for(int n = 0; n < 4; n++){
        digitalWrite(S[n], !!(i & (0x1 << n)));
      //  Serial.print(!!(i & (0x1 << n)));
      }
      //Serial.print("\t");
      values[i] = analogRead(PIN_MUX_AN);
    }
  }
  int getValue(int i){
  return values[i];
}

float getPosition(){
  float weighted_sum = 0;
  float sum = 0;
  for(int i = ini; i < fim; i++){
    float value = (getValue(i) - 0.0) * (-1.0) / (SENSOR_MAX - SENSOR_MIN) + 1.0;
    value = fmin(fmax(0.0, value), 1.0);
    Serial.print(i);
    Serial.print(" : ");
    Serial.print(value);
    Serial.print(" ");
    weighted_sum = weighted_sum + value  * (i -7);
    sum = sum + value;
  }
  float position = ((weighted_sum / sum) - 4.5) / 3.5;
  return position;
}

void calibrateSensor(){
  int min = 1024;
  int max = -1;
  for(int n = 0; n < 10; n++){
    readValues();
    for(int i = 0; i < 8; i++){
      int value = getValue(i);
      if(min > value){
        min = value;
      }
      if(max < value){
        max = value; 
      }
    }      
  }
  SENSOR_MIN = min;
  SENSOR_MAX = max;    
}

void setup() {
  Serial.begin(9600);
  pinMode(PIN_MUX_ENABLE, OUTPUT);
  pinMode(PIN_MUX_S0, OUTPUT);
  pinMode(PIN_MUX_S1, OUTPUT);
  pinMode(PIN_MUX_S2, OUTPUT);
  pinMode(PIN_MUX_S3, OUTPUT);
  pinMode(PIN_MUX_AN, INPUT);
  //calibrateSensor();
  
}

void loop() {
  digitalWrite(PIN_MUX_ENABLE, HIGH);
  digitalWrite(PIN_MUX_S0, HIGH);
  readValues();
  Serial.print(getPosition());
  Serial.println();
  delay(100);
  
  
}
