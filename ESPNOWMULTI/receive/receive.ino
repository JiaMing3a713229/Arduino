//引入函式庫
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
//固定變數
#define NUMSLAVES 20
#define CHANNEL 0
#define PRINTSCANRESULTS 0
//ESP32掃描
esp_now_peer_info_t peerInfo[NUMSLAVES] = {};
int SlaveCnt = 0;

int number;
//設置變數
#define board_number 1

typedef struct struct_message {
  String data_number;
  String board_name;
  
  } struct_message;
struct_message myData[20];
struct_message database;

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
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {

  
  memcpy(&database, incomingData, sizeof(database));
  Serial.printf("number:%s ,from:%s\n",database.data_number,database.board_name);

  
}
void setup() {
  
  Serial.begin(115200);

  configDeviceAP();
  WiFi.mode(WIFI_AP_STA);
  InitESPNow();

   // 設定傳送資料回撥函式
  //esp_now_register_send_cb(OnDataSent);
  delay(10);
  // 設定接收資料回撥函式
  esp_now_register_recv_cb(OnDataRecv);

  myData[board_number].data_number="0";
  number=0;
  myData[board_number].board_name = "WiFi_#"+String(board_number);
  
  
  //ScanForSlave();
  //manageSlave();
  delay(1000);
}

void loop() {
  vTaskDelay(10 / portTICK_PERIOD_MS);    // 延遲1000ms
  /*
  if(ScanForSlave>0){
   
      if(Esp_now_datasend()){
        number+=1;
        myData[board_number].data_number = String(number);
        Serial.printf("number:%s ,Send:%s\n",myData[board_number].data_number,myData[board_number].board_name);
        
      }
    
    }
    
    
    vTaskDelay(1000 / portTICK_PERIOD_MS);    // 延遲1000ms
    */

}

void ScanForSlave() {
  int8_t scanResults = WiFi.scanNetworks();
  
  //reset slaves
  memset(peerInfo, 0, sizeof(peerInfo));
  SlaveCnt = 0;
  Serial.println("");
  if (scanResults == 0) {
    Serial.println("No WiFi devices in AP Mode found");
  } 
  else {
    Serial.print("Found "); Serial.print(scanResults); Serial.println(" devices ");
    for (int i = 0; i < scanResults; ++i) {
      // Print SSID and RSSI for each device found
      String SSID = WiFi.SSID(i);
      int32_t RSSI = WiFi.RSSI(i);
      String BSSIDstr = WiFi.BSSIDstr(i);
      
      vTaskDelay(10 / portTICK_PERIOD_MS);  // 延遲500ms
      // Check if the current device starts with `Slave`
      if (SSID.indexOf("Slave") == 0) {
        // SSID of interest
        Serial.print(i + 1); Serial.print(": "); Serial.print(SSID); Serial.print(" ["); Serial.print(BSSIDstr); Serial.print("]"); Serial.print(" ("); Serial.print(RSSI); Serial.print(")"); Serial.println("");
        // Get BSSID => Mac Address of the Slave
        int mac[6];
        //Serial.print("BSSIDstr:");
        //Serial.println(BSSIDstr);
        
        if ( 6 == sscanf(BSSIDstr.c_str(), "%x:%x:%x:%x:%x:%x",  &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5] ) ) {
          
          for (int ii = 0; ii < 6; ii++ ) {
          
            peerInfo[SlaveCnt].peer_addr[ii] = (uint8_t) mac[ii];
             
          }
          peerInfo[SlaveCnt].channel = CHANNEL; // pick a channel
          peerInfo[SlaveCnt].encrypt = 0; // no encryption
          SlaveCnt++;
        }
      
      }
    }
  }

  
  if (SlaveCnt > 0) {
    //Serial.print(SlaveCnt); Serial.println(" Slave(s) found, processing..");
  } else {
    Serial.println("No Slave Found, trying again.");
  }

  // clean up ram
  WiFi.scanDelete();
}

void manageSlave() {
  
  if (SlaveCnt > 0) {
    for(int i=0;i<SlaveCnt;i++){

      bool exists = esp_now_is_peer_exist(peerInfo[i].peer_addr);
      if (exists) {
        // Slave already paired.
        Serial.println("Already Paired");
      }
      else{
        esp_err_t addStatus = esp_now_add_peer(&peerInfo[i]);
        if (addStatus == ESP_OK) {
          // Pair success
          Serial.println("Pair success");
        } 
        
        else if (addStatus == ESP_ERR_ESPNOW_NOT_INIT) {
          // How did we get so far!!
          //Serial.println("ESPNOW Not Init");
        } else if (addStatus == ESP_ERR_ESPNOW_ARG) {
          //Serial.println("Add Peer - Invalid Argument");
        } else if (addStatus == ESP_ERR_ESPNOW_FULL) {
          //Serial.println("Peer list full");
        } else if (addStatus == ESP_ERR_ESPNOW_NO_MEM) {
          //Serial.println("Out of memory");
        } else if (addStatus == ESP_ERR_ESPNOW_EXIST) {
          //Serial.println("Peer Exists");
        } else {
          //Serial.println("Not sure what happened");
        }
        
      }
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);  
    
    

  } 
  else {
    // No slave found to process
    Serial.println("No Slave found to process");
  }
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

void configDeviceAP() {
  String Prefix = "Slave:";
  String Mac = WiFi.macAddress();
  String SSID = Prefix + Mac;
  String Password = "123456789";
  bool result = WiFi.softAP(SSID.c_str(), Password.c_str(), CHANNEL, 0);
  if (!result) {
    Serial.println("AP Config failed.");
  } else {
    Serial.println("AP Config Success. Broadcasting with AP: " + String(SSID));
  }
}

bool Esp_now_datasend(){

  //const uint8_t *peer_addr = peerInfo[i].peer_addr;
  esp_err_t result = esp_now_send(0,(uint8_t *) &myData[board_number], sizeof(myData[board_number]));
  Serial.print("Send Status: ");
    if (result == ESP_OK) {
      Serial.println("Success");
      return true;
    } 
    
}
