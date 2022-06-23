
portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
volatile bool Led1State = 0;
bool Led2State = 0;
hw_timer_t * timer;

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
void setup() {

  Serial.begin(115200);
  pinMode(LED1PIN, OUTPUT);
  pinMode(LED2PIN, OUTPUT);
  
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &TimerISR, true);
  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);
  delay(1000);
}

void loop() {
  
  delay(2000);
  Led2State = !Led2State;
  Serial.println(Led2State);
  digitalWrite(LED2PIN,Led2State);
  
}
