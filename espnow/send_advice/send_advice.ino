#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
 
typedef struct message {
  String send_number;
};
 
struct message myMessage;

int number;
uint8_t peer1[] = {0x34, 0xAB, 0x95, 0x73, 0x92, 0x28}; //接收端的MAC位址(自行修改)
esp_now_peer_info_t peerInfo;

//void onSent(uint8_t *mac_addr, uint8_t sendStatus) {
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  //Serial.print("Packet to: ");
  // Copies the sender mac address to a string
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  
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

 
/*##################################################################################################

##################################################################################################*/
void setup() {
  Serial.begin(115200);
  
  
  
  WiFi.mode(WIFI_STA);
  
  Serial.print("Mac Address: ");
  Serial.print(WiFi.macAddress());
  Serial.println("ESP32 ESP-Now Broadcast");
  
  // Initializing the ESP-NOW
  InitESPNow();
 
  
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  
  memcpy(peerInfo.peer_addr, peer1, 6);
  
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }  
  
  //******************************************************************
  number = 0;
  
}
 
/*##################################################################################################

##################################################################################################*/
void loop() {

  number++;
  myMessage.send_number = String(number);
  esp_err_t result = esp_now_send(0, (uint8_t *) &myMessage, sizeof(myMessage));
  if (result == ESP_OK) {
    Serial.println("Send Success");
  }
  delay(1000);
 
 
}
