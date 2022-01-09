#define LEDPIN 2
boolean ledstate=LOW;
void setup() {
  pinMode(LEDPIN,OUTPUT);
  Serial.begin(115200);
}

void loop() {
  
  delay(1000);
  digitalWrite(LEDPIN,HIGH);
  delay(100);
  digitalWrite(LEDPIN,LOW);
  
}
