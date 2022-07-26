#include <Arduino.h>
#include <WebServer.h>

WebServer server(80);

#define LED1 2
bool LEDState = LOW;

const char *ssid = "jiaphone";
const char *pass = "jia418009";

String updatepage(uint8_t LEDState){
  String ptr = "<!DOCTYPE html>\n";
  ptr += "<html lang=\"en\">\n";
  ptr += "<head>\n";
  ptr += "<meta charset=\"UTF-8\">\n";
  ptr += "<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n";
  ptr += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
  ptr += "<title>Led Control page</title>\n";
  ptr += "<style>\n";
  ptr += "html {font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} \n";
  ptr += "h1 {color: #444444;margin: 50px auto 30px;} \n";
  ptr += "h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr += ".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr += ".button-on {background-color: #3498db;}\n";
  ptr += ".button-on:active {background-color: #3498db;}\n";
  ptr += ".button-off {background-color: #34495e;}\n";
  ptr += ".button-off:active {background-color: #2c3e50;}\n";
  ptr += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>JM ESP32 WebServer</h1>\n";
  ptr += "<h3>Control Led state</h3>\n";
  if(LEDState){
    ptr += "<p>LED:ON</p><a class=\"button button-off\" href=\"/ledoff\">OFF</a>\n";
  }
  else{
    ptr += "<p>LED:OFF</p><a class=\"button button-on\" href=\"/ledon\">ON</a>\n";
  }

  ptr += "</body>\n";
  ptr += "</html>\n";

  return ptr;

}


void HandelRoot(){
  String HTML = "<DOCTYPE html>\
    <html>\
      <head><meta charset='utf-8'></head>\
      <body>ESP32 HELLO WORLD</body>\
    </html>";
  server.send(200, "text/html", HTML);
}
void Handleledon(){
  LEDState = HIGH;
  Serial.println("LED:ON");
  server.send(200,"text/html",updatepage(LEDState));
}
void Handleledoff(){
  LEDState = LOW;
  Serial.println("LED:OFF");
  server.send(200,"text/html",updatepage(LEDState));
}


void setup(){
  Serial.begin(115200);

  pinMode(LED1, OUTPUT);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    vTaskDelay(10/portTICK_RATE_MS);
  }
  Serial.println("");
  Serial.print("Successed connect WiFi IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", HandelRoot);
  server.on("/about",[](){
    server.send(200, "text/html; charset=utf-8", "Welcome JM Learning channel");
  });
  server.on("/ledon", Handleledon);
  server.on("/ledoff",Handleledoff);
  server.onNotFound([](){
    server.send(404, "text/plain","Not found");
  });

  server.begin();
}


void loop(){
  server.handleClient();

  if(LEDState){
    digitalWrite(LED1, HIGH);
  }
  else{
    digitalWrite(LED1, LOW);
  }
}

