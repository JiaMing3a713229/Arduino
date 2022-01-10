#include<WiFi.h>

const char* ssid = "jiaming";
const char* pass = "ty418009";

void setup() {

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,pass);
  while(WiFi.status()!=WL_CONNECTED){
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connect to WiFi");
  Serial.print("local IP is:");
  Serial.println(WiFi.localIP());


}

void loop() {
  // put your main code here, to run repeatedly:

}
