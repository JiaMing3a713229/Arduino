#include <Arduino.h>
#include "WebServer.h"

WebServer server(80); //於ESP32上宣告WerServer Handle 並設置端口(port)為80

const char* ssid = "jiaphone";  //WIFI帳號
const char* pass = "jia418009"; //WIFI密碼

//於瀏覽器上顯示HTML檔案，寫著hello word
void HandleRoot(){ 
  String HTML = "<!DOCTYPE html>\
  <html>\
    <head><meta charset='utf-8'></head>\
    <body>Welcome JM Learning !!</body>\
  </html>";
  server.send(200, "text/html", HTML);
}

void setup() {

  Serial.begin(115200);
  WiFi.begin(ssid, pass); //WIFI連線
  while(WiFi.status() !=WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.print("IP:");
  Serial.println(WiFi.localIP());  //顯示esp32於wifi連線後，STA模式下的IP位址


  server.on("/", HandleRoot);  //客戶端對Server端的根路徑"/"發起request請求後，Server將對客戶端回復response所對應的回調函式

  server.on("/about", [](){    //客戶端對Server端的根路徑"/about"發起request請求後，Server將對客戶端提供一個html資料，寫著歡迎來到esp32 Web Server!!
    server.send(200, "text/html; charset=utf-8","歡迎來到esp32 Web Server!!");
  });

  server.onNotFound([](){      //Server辨識不出來客戶端請求的根路徑，所對應的response，Server將對客戶端提供一個html資料，寫著File NOT found!
    server.send(404, "text/plain", "File NOT found!");
  });

  server.begin();  //於esp32上啟動伺服器

}

void loop() {
  server.handleClient(); //處理客戶端的請求
}