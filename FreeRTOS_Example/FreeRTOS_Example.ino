#define LED1 32
#define LED2 33



void LED1Task(void *pvParam){
  pinMode(LED1,OUTPUT);
  vTaskDelay(1000/portTICK_PERIOD_MS);
  while(1){
    Serial.println("LED1Task");
    digitalWrite(LED1,1);
    vTaskDelay(1000/portTICK_PERIOD_MS);
    digitalWrite(LED1,0);
    vTaskDelay(1000/portTICK_PERIOD_MS);
  }
}

void LED2Task(void *pvParam){
  pinMode(LED2,OUTPUT);
  vTaskDelay(1000/portTICK_PERIOD_MS);
  while(1){
    Serial.println("LED2Task");
    digitalWrite(LED2,1);
    vTaskDelay(1000/portTICK_PERIOD_MS);
    digitalWrite(LED2,0);
    vTaskDelay(1000/portTICK_PERIOD_MS);
  }
}
void setup() {

  Serial.begin(115200);

  //建立任務，並加入至系統排程器
  xTaskCreate(LED1Task,"LED1Task",1024,NULL,1,NULL);
  xTaskCreate(LED2Task,"LED2Task",1024,NULL,1,NULL);
  /*
   1.指定任務函式
   2.描述任務名稱、字串
   3.分配此任務Stack記憶體大小
   4.傳給任務的參數
   5.優先序 數字越大，優先等級越高，最高為configMAX_PRIORITIES-1，所以為24
   6.任務參照，用於任務的深入排定
   */
  vTaskDelay(2000/portTICK_PERIOD_MS);
}

void loop() {
  

}
