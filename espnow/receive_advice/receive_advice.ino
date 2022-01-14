#include <WiFi.h>
#include <esp_now.h>

 
typedef struct message {
  String send_number;
};
 
struct message myMessage;

//uint8_t peer1[] = {0x30, 0x83, 0x98, 0x57, 0x06, 0xec}; //接收端的MAC位址(自行修改)
esp_now_peer_info_t peerInfo;


void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  
  memcpy(&myMessage, incomingData, sizeof(myMessage));
  Serial.printf("Receive message:%s\n",myMessage.send_number);

  
  
}
void InitESPNow() {
  WiFi.disconnect();
  if (esp_now_init() == ESP_OK) {
    Serial.println("ESPNow Init Success");
  }
  else {
    Serial.println("ESPNow Init Failed");
    // Retry InitESPNow, add a counte and then restart?
    // InitESPNow();
    // or Simply Restart
    ESP.restart();
  }
}

 
void setup() {
  Serial.begin(115200);
  
  
  
  WiFi.mode(WIFI_STA);
  
  Serial.print("Mac Address: ");
  Serial.print(WiFi.macAddress());
  Serial.println("ESP32 ESP-Now Broadcast");
  
  // Initializing the ESP-NOW
  InitESPNow();
  //peerInfo.channel = 0;  
  //peerInfo.encrypt = false;
  
  esp_now_register_recv_cb(OnDataRecv);
 
  
}
 
/*##################################################################################################

##################################################################################################*/
void loop() {
 
}
