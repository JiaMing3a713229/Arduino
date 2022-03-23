HardwareSerial uart1(1);

#define TXD_PIN 17
#define RXD_PIN 16


void setup() {
  Serial.begin(115200);
  uart1.begin(115200,SERIAL_8N1,RXD_PIN,TXD_PIN);
 
}

void loop() {
  if(uart1.available()>0){
    String mag = uart1.readString();
    Serial.printf("收到資料:%s\n",mag);
  }
  delay(10);
}
