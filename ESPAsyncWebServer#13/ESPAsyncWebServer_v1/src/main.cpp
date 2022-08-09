#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <SPIFFS.h>

#define DHTPIN 32        //以GPIP32讀取DHT11資料
#define DHTTYPE DHT11    
#define LED1PIN 2

DHT dht(DHTPIN, DHTTYPE);  //建立DHT11 資料腳位為GPIO32
AsyncWebServer server(80); //建立端口為80的伺服器物件

//****用於WiFi連線之識別名稱、密碼*************************
const char *ssid = "jiaphone"; //WiFi識別名稱
const char *pass = "jia418009"; //WiFi密碼

//****DHT11讀取溫度***************************************
/*
  API:
      readTemperature() 返回DHT11之溫度
      readHumidity()    返回DHT11之濕度
*/
String ReadTemperature(){
  float temperature = dht.readTemperature();
  if(isnan(temperature)){
    //Serial.println("讀取dht11 溫度 失敗");
    return "";
  }
  return String(int(temperature));
}
String ReadHumidity(){
  float h = dht.readHumidity();
  if(isnan(h)){
    //Serial.println("dht11讀取濕度 失敗");
    return "";
  }
  return String(int(h));
}
//**********************************************************
void setup(){

  Serial.begin(115200);
  dht.begin();  //初始化DHT11並開始讀取資料
  pinMode(LED1PIN, OUTPUT);//GPIO2為輸出模式
  
  //****掛載ESP32 SPIFFS文件系統*****************************
  if(!SPIFFS.begin(true)){
    Serial.println("SPIFFS 掛載失敗");
  }
  else if(SPIFFS.begin(true)){
    Serial.println("成功掛載SPIFFS");
  }
  //*********************************************************
  //WiFi連線*************************************************
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while(WiFi.status() != WL_CONNECTED){
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
  if(WiFi.status() == WL_CONNECTED){
    Serial.println("WiFi 連線成功");
    Serial.print("local IP:");
    Serial.print(WiFi.localIP());
    Serial.print("  RSSI:");
    Serial.println(WiFi.RSSI());
  }
  //*********************************************************
  //註冊Web Server收到特定請求後，執行所對應的回調函式***********

  server.on("/text", HTTP_GET, [](AsyncWebServerRequest *request){
    String t = ReadTemperature();
    String h = ReadHumidity();
    request -> send(200, "text/html", "JM Learning Hello world");
  });
  server.on("/monitor", HTTP_GET, [](AsyncWebServerRequest *request){
    request -> send(SPIFFS, "/monitor.html");
  });

  //從Server端讀值***********************************************************************
  server.on("/Temp", HTTP_GET, [](AsyncWebServerRequest *request){
    String t = ReadTemperature();
    request -> send(200, "text/html", t);
  });
  server.on("/Humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    String h = ReadHumidity();
    request -> send(200, "text/html", h);
  });
  server.on("/ledon", HTTP_GET, [](AsyncWebServerRequest *request){
    String LED1State = "ON";
    digitalWrite(LED1PIN,HIGH);
    request -> send(200, "text/html", LED1State);
  });
    server.on("/ledoff", HTTP_GET, [](AsyncWebServerRequest *request){
    String LED1State = "OFF";
    digitalWrite(LED1PIN,LOW);
    request -> send(200, "text/html", LED1State);
  });
  //**************************************************************************************
  //開啟端口為80的AsyncWebServer，並進行監聽
  server.begin();

}

void loop(){
}