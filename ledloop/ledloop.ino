#define LED1 32
#define LED2 33

void LED1Task(){

  Serial.println("LED1Task");
  digitalWrite(LED1,1);
  vTaskDelay(1000/portTICK_PERIOD_MS);
  digitalWrite(LED1,0);
  vTaskDelay(1000/portTICK_PERIOD_MS);
  
}

void LED2Task(){

  Serial.println("LED2Task");
  digitalWrite(LED2,1);
  vTaskDelay(1000/portTICK_PERIOD_MS);
  digitalWrite(LED2,0);
  vTaskDelay(1000/portTICK_PERIOD_MS);
  
}
void setup() {
  Serial.begin(115200);
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  
}
void loop() {
  
  LED1Task();
  LED2Task();

}
