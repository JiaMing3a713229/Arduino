portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE mux1 = portMUX_INITIALIZER_UNLOCKED;

#define LED_BUILTIN 2
#define LED2 33
#define LED1_DELAY 5
#define LED2_DELAY 3

volatile bool isLED1 = 0;
volatile bool isLED2 = 0;




hw_timer_t * timer1;
hw_timer_t * timer2;

void IRAM_ATTR onTimer1(){
  portENTER_CRITICAL(&mux);
  isLED1 = !isLED1;
  digitalWrite(LED_BUILTIN, isLED1);
  portEXIT_CRITICAL(&mux);
}

void IRAM_ATTR onTimer2(){
  portENTER_CRITICAL(&mux1);
  isLED2 = !isLED2;
  digitalWrite(LED_BUILTIN, isLED2);
  portEXIT_CRITICAL(&mux1);
}





void setup() {

  
  Serial.begin(115200);
  
  pinMode(LED_BUILTIN,OUTPUT);
  pinMode(LED2,OUTPUT);

  timer1 = timerBegin(0,80,true);
  timerAttachInterrupt(timer1,&onTimer1,true);
  timerAlarmWrite(timer1,1000000*LED1_DELAY,true);
  timerAlarmEnable(timer1);

  timer2 = timerBegin(1,80,true);
  timerAttachInterrupt(timer2,&onTimer2,true);
  timerAlarmWrite(timer2,1000000*LED2_DELAY,true);
  timerAlarmEnable(timer2);

}

void loop() {
  
}
