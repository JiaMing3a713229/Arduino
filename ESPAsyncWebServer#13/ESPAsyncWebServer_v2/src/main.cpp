#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <dht.h>
#include <Adafruit_Sensor.h>
#include <SPIFFS.h>

#define DHTTYPE DHT11
#define DHTPIN 32
#define LEDPIN 2  //內部LED 
DHT dht(DHTPIN, DHTTYPE); //宣告類別為DHT物件，"dht11"會根據"GPIO32"來讀取資料
AsyncWebServer server(80);//宣告端口為80的AsyncWebServer類別物件，

//根據手邊有的WiFi來設置
const char *ssid = "jiaphone"; //WiFi 識別名稱
const char *pass = "jia418009"; //WiFi 密碼

//將dht11所讀取的浮點數類型資料轉為字串資料
String ReadTemperature(){
  float t = dht.readTemperature();
  if(isnan(t)){
    Serial.println("讀取DHT11 讀取溫度失敗");
    return "";
  }
  return String(int(t)); //將浮點數轉為整數再轉為字串
}

String ReadHumidity(){
  float h = dht.readHumidity();
  if(isnan(h)){
    Serial.println("DHT11 濕度讀取失敗");
    return "";
  }
  return String(int(h));
}


void setup() {
  
  Serial.begin(115200);
  dht.begin();
  pinMode(LEDPIN,OUTPUT);
  if(SPIFFS.begin(true)){
    Serial.println("SPIFFS 掛載成功");
  }
  else{
    Serial.println("SPIFFS 掛載失敗");
  }

  //****WiFi連線*******************************************
  WiFi.begin(ssid, pass);
  while(WiFi.status() != WL_CONNECTED){
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
  if(WiFi.status() == WL_CONNECTED){
    Serial.println("連線成功");
    Serial.print("IP Address:");
    Serial.print(WiFi.localIP());
    Serial.print(" RSSI:");
    Serial.println(WiFi.RSSI());
  }

  //Server請求路徑，以及相對應的響應方式

  server.on("/test",HTTP_GET,[](AsyncWebServerRequest *request){
    request -> send(200, "text/html","JM Learning Hello world");
  });

  server.on("/",HTTP_GET,[](AsyncWebServerRequest *request){
    request -> send(SPIFFS,"/index.html"); //對請求/monitor的客戶端響應一個位於SPIFFS中，路徑為/index的html網頁
  });

  server.on("/monitor",HTTP_GET,[](AsyncWebServerRequest *request){
    request -> send(SPIFFS,"/monitor.html"); //對請求/monitor的客戶端響應一個位於SPIFFS中，路徑為/monitor的html網頁
  });

  //客戶端對伺服器發起HTTP GET的請求後，響應對應資料**********************
  server.on("/Temp",HTTP_GET,[](AsyncWebServerRequest *request){
    String temp = ReadTemperature();
    request -> send(200, "text/html", temp);
  });
  server.on("/Humidity",HTTP_GET,[](AsyncWebServerRequest *request){
    String humidity = ReadHumidity();
    request -> send(200, "text/html", humidity);
  });
  server.on("/ledon",HTTP_GET,[](AsyncWebServerRequest *request){
    digitalWrite(LEDPIN,HIGH);
    request -> send(200, "text/html", "on");
  });
  server.on("/ledoff",HTTP_GET,[](AsyncWebServerRequest *request){
    digitalWrite(LEDPIN,LOW);
    request -> send(200, "text/html", "off");
  });



  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
}