//引入ble協定下所需的LIB------------------------------
#include <esp_bt_device.h>
#include <BLEDevice.h> //建立及初始化BLE裝置
#include <BLEUtils.h>  //包含轉換類型的Lib
#include <BLEServer.h> //建立BLE伺服器
#include <BLE2902.h>   //推送通知的描述(UUID:0x2902)
//GATT協議下所需的特徵UUID----------------------------
//Service服務之UUID，由128bit組成 32個16進位資料
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
//Characteristic之UUID，，由128bit組成 32個16進位資料
#define CHARACTERISTIC_UUID "6e400003-b5a3-f393-e0a9-e50e24dcca9e"
//宣告GATT之服務、特徵，為指標型態資料
static BLEService *pService;
static BLECharacteristic *CountCharacteristic;
//計數資料
uint32_t number;
//一開始無連線
bool bleConnected = false;
//BLE連線回調函式
class ServerCallbacks : public BLEServerCallbacks
{
  void onConnect(BLEServer *pServer)
  {
    bleConnected = true;
  };

  void onDisconnect(BLEServer *pServer)
  { 
    // 斷線回呼
    bleConnected = false;
    Serial.println("連線中斷");
    BLEDevice::startAdvertising(); // 重新發出廣告
  }
};
//列出手中NodeMCU 32S之藍芽MAC位址
void printDeviceAddress() {
  const uint8_t* point = esp_bt_dev_get_address();
  for (int i = 0; i < 6; i++) {
    char str[3];
    sprintf(str, "%02X", (int)point[i]);
    Serial.print(str);
 
    if (i < 5){
      Serial.print(":");
    }
  }
}

void setup() {

  Serial.begin(115200);
  Serial.println("開始啟動BLE裝置!");

  BLEDevice::init("ESP32 Server_dev");//BLE裝置名稱
  BLEServer *pServer = BLEDevice::createServer();//初始化、並啟動BLE Server功能
  pServer->setCallbacks(new ServerCallbacks()); //設置伺服器回調函式為ServerCallbacks
  BLEService *pService = pServer->createService(SERVICE_UUID);//BLE Server 啟動服務並設置指定UUID
  CountCharacteristic = pService->createCharacteristic(         //BLE 服務的每筆資料稱特徵Characteristic，設置其中的"UUID"、"屬性"
                         CHARACTERISTIC_UUID,
                         BLECharacteristic::PROPERTY_NOTIFY
                       );
  
          
  CountCharacteristic->addDescriptor(new BLE2902()); // 新增描述
  

  BLEDescriptor *pDesc = new BLEDescriptor((uint16_t)0x2901);
  pDesc->setValue("數值計數");//描述內容
  CountCharacteristic->addDescriptor(pDesc);//對特徵增加描述內容
  // 啟動服務
  pService->start();
  //開始對外廣播BLE SERVER
  pServer->getAdvertising()->start();
  //設置廣播內容
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x06);  
  pAdvertising->setMinPreferred(0x12);
  // 開始對外廣播
  BLEDevice::startAdvertising();
  Serial.println("等待用戶端連線…");
  printDeviceAddress();
  number = 0;
}

void loop() {
  
  if(bleConnected){
    
      
      String data_send = String(number);
      Serial.printf("發送:%s\n",data_send);
      CountCharacteristic->setValue(data_send.c_str());
      CountCharacteristic->notify();
      number++;
    
    }
  else{
    number = 0;
  }
    
    delay(2000);
}
