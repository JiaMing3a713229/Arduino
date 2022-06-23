#include<DHT.h>
#define DHTPIN 33
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE mux1 = portMUX_INITIALIZER_UNLOCKED;

volatile bool Led1State = 0;
bool Led2State = 0;

hw_timer_t * timer;
hw_timer_t * timer2;

//LED1會在計時中斷觸發時閃爍，LED2會在LOOP中一直閃爍
#define LED1PIN 2
#define LED2PIN 32



void IRAM_ATTR TimerISR(){
  //注意，在中斷中不要出現Serial.print
  
  portENTER_CRITICAL(&mux);

  Led1State = !Led1State;
  digitalWrite(LED1PIN,Led1State);

  portEXIT_CRITICAL(&mux);
  
}

void IRAM_ATTR Timer2ISR(){
  //注意，在中斷中不要出現Serial.print
  
  portENTER_CRITICAL(&mux1);

  Led2State = !Led2State;
  digitalWrite(LED2PIN,Led2State);

  portEXIT_CRITICAL(&mux1);
  
}
void setup() {

  Serial.begin(115200);
  pinMode(LED1PIN, OUTPUT);
  pinMode(LED2PIN, OUTPUT);
  dht.begin();
  
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &TimerISR, true);
  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);

  timer2 = timerBegin(1,80,true);
  timerAttachInterrupt(timer2,&Timer2ISR,true);
  timerAlarmWrite(timer2,1000000*2,true);
  timerAlarmEnable(timer2);
  
  delay(1000);
}

void loop() {
  
  delay(5000);
  float h = dht.readHumidity();   //取得濕度
  float t = dht.readTemperature();  //取得溫度C

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C ");
  
}
