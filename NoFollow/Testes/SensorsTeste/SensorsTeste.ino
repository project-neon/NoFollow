#define PIN_MUX_S0 6
#define PIN_MUX_S1 12
#define PIN_MUX_S2 7
#define PIN_MUX_S3 4
#define PIN_MUX_AN 5
#define PIN_MUX_ENABLE 16
int values[16];
static int S[] = {PIN_MUX_S0, PIN_MUX_S1, PIN_MUX_S2, PIN_MUX_S3};
  
void readValues(){
    for(uint8_t i = 0; i<16; i++){
      for(int n = 0; i<4; i++){
        digitalWrite(S[n], i&(0x1<<n));
      }
      values [i] = analogRead(PIN_MUX_AN);
    }
  }
  int getValue(int i){
    return values[i];
  }

void setup() {
  Serial.begin(9600);
  pinMode(PIN_MUX_ENABLE, OUTPUT);
  pinMode(PIN_MUX_S0, OUTPUT);
  pinMode(PIN_MUX_S1, OUTPUT);
  pinMode(PIN_MUX_S2, OUTPUT);
  pinMode(PIN_MUX_S3, OUTPUT);
  pinMode(PIN_MUX_AN, INPUT);
}

void loop() {
  digitalWrite(PIN_MUX_ENABLE, HIGH);
  readValues();
  for(int i =0; i<16;i++){
    Serial.print("\t");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(getValue(i));
  }
  Serial.println();
  delay(300);
  
  
}
