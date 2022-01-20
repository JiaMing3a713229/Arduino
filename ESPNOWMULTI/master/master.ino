#include<esp_now.h>
#include<WiFi.h>

//number of Slave
#define NUMSLAVES 20
esp_now_peer_info_t peers[NUMSLAVES]={};
int SlaveCnt = 0;

#define CHANNEL 0
#define PRINTSCANRESULTS 0
#define BOARDNUMBER 0
int number = 0;
typedef struct structMessage{
  String data_number;
  String board_name;
}structMessage;

structMessage myData[NUMSLAVES];

void InitESPNow(){
  WiFi.disconnect();
  if(esp_now_init() == ESP_OK){
    Serial.println("ESP NOW Init Sucess");
    esp_now_register_send_cb(OnDataSent);
  }
  else{
    Serial.println("ESP NOW Init Fail");
    ESP.restart();
  }
}
void ConfigDeviceAP(){
  
  String prefix = "Slave:"; //SSID特定字首
  String mac = WiFi.macAddress();
  String SSID = prefix+mac; 
  String Password = "123456789";
  
  bool result = WiFi.softAP(SSID.c_str(), Password.c_str(), CHANNEL, 0);
  
  if(!result){
    Serial.println("AP Config failed.");
  }
  else{
    Serial.println("AP Config Success.Broadcasting with AP:" + String(SSID));
    
  }
  delay(100);
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status){

  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
            mac_addr[0],mac_addr[1],mac_addr[2],mac_addr[3],mac_addr[4],mac_addr[5]);
            
  Serial.print("Last packet sent to:");
  Serial.println(macStr);
  Serial.print("Send status:");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail"); 
}
void ScanForSlave(){
  
  memset(peers, 0, sizeof(peers));
  int8_t scanresults = WiFi.scanNetworks();
  SlaveCnt = 0;
  Serial.printf("Found %d AP devices\n",scanresults);
  if(scanresults==0){
    Serial.println("NO WIFI device in AP Mode");
  }
  else{
    
    for(int i=0;i<scanresults;++i){
      int32_t RSSI = WiFi.RSSI(i);
      String SSID = WiFi.SSID(i);
      String BSSIDstr = WiFi.BSSIDstr(i);
      delay(10);
      if(SSID.indexOf("Slave")==0){
        
        Serial.print(i+1);
        Serial.print(":");
        Serial.print("[ ");
        Serial.print(BSSIDstr);
        Serial.print(" ]");
        Serial.print("   (");
        Serial.print(RSSI);
        Serial.println(")");
        
        //Serial.printf("%d [%s] RSSI:(%d)\n",i+1, BSSIDstr, RSSI);

        int mac[6];

        if(6 == sscanf(BSSIDstr.c_str(),"%x:%x:%x:%x:%x:%x",  &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5])){
          for(int ii=0;ii<6;++ii){
            peers[SlaveCnt].peer_addr[ii] = (uint8_t)mac[ii];
          }
        }
        peers[SlaveCnt].channel = CHANNEL;
        peers[SlaveCnt].encrypt = 0;
        SlaveCnt++;
      }
      
    }
  }
  if(SlaveCnt>0){
    Serial.print(SlaveCnt); 
    Serial.println(" Slave(s) found, processing..");
  }
  else{
    Serial.println("No Slave Found, trying again.");
  }
  
  WiFi.scanDelete();
}

void manageSlave(){

  if(SlaveCnt>0){
    for(int i=0;i<SlaveCnt;i++){
      Serial.print("Processing: ");
      for(int ii=0;ii<6;ii++){
        Serial.print((uint8_t)peers[i].peer_addr[ii],HEX);
        if(ii!=5){
          Serial.print(":");
        }
      }
      Serial.print("Status:");

      bool exists = esp_now_is_peer_exist(peers[i].peer_addr);
      if(exists){
        Serial.println("Already Paird");
      }
      else{
        
        esp_err_t addStatus = esp_now_add_peer(&peers[i]);
        if(addStatus == ESP_OK){
          Serial.println("Pair success");
        }
      }
      delay(100);
    }
  }
  else{
    Serial.println("No Slave found to process");
  }
}

void send_data(){
  
  ++number;
  myData[BOARDNUMBER].data_number = String(number);
  
  for(int i=0;i<SlaveCnt;i++){
    const uint8_t *peer_addr = peers[i].peer_addr;
    if(i==0){
      Serial.print("Sending:");
      Serial.println(number);
    }
    esp_err_t result = esp_now_send(peer_addr,(uint8_t *)&myData[BOARDNUMBER],sizeof(myData[BOARDNUMBER]));
    if(result!=ESP_OK){
      Serial.println("Send failed");
    }
    else{
      Serial.println("Send Success");
    }
    delay(10);
  }
}
void setup() {
  
  Serial.begin(115200);
  WiFi.mode(WIFI_AP_STA);
  ConfigDeviceAP();
  InitESPNow();
  
  //esp_now_register_send_cb(OnDataSent);

  ScanForSlave();

  if(SlaveCnt>0){
    manageSlave();
  }

  myData[BOARDNUMBER].data_number = "";
  myData[BOARDNUMBER].board_name = "WiFi_#"+String(BOARDNUMBER);
  delay(1000);
  
  
}

void loop() {

  
  send_data();
  delay(1000);

}
