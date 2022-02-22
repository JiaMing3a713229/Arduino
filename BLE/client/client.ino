#include "BLEDevice.h"

//我們希望連上的服務
static BLEUUID serviceUUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b");
//我們希望連上的特徵
static BLEUUID charUUID1("6e400003-b5a3-f393-e0a9-e50e24dcca9e");
//判斷是否連線、完成掃描
static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;


static BLERemoteCharacteristic* pRemoteCharacteristic;
static BLEAdvertisedDevice* myDevice;

//BLE 通知的回調函式，如果有資料則儲存並列印
static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
    
    Serial.print("Notify callback for characteristic ");
    Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
    
    Serial.print("資料長度");
    Serial.println(length);
    String Collect_Date = (char*)pData;
    Serial.print("收到資料: ");
    Serial.println(Collect_Date);

    Collect_Date = "";
    
}
//連線之回調函式，會返回連線狀態
class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {

  }

  void onDisconnect(BLEClient* pclient) {
    connected = false;
    Serial.println("onDisconnect");
  }
};
//與Server連線，並判斷是否成功，回傳bool
bool connectToServer() {
  
    Serial.print("Forming a connection to ");
    Serial.println(myDevice->getAddress().toString().c_str());
    //啟動BLE GATT Client
    BLEClient*  pClient  = BLEDevice::createClient();
    Serial.println("啟動Client!");
    //設置連線之回調函數
    pClient->setClientCallbacks(new MyClientCallback());
    //與BLE Server連線
    pClient->connect(myDevice);  
    Serial.println(" - Connected to server");
    //設置最大資料傳輸長度517位元組
    //set client to request maximum MTU from server (default is 23 otherwise)
    pClient->setMTU(517); 
  
    //Obtain a reference to the service we are after in the remote BLE server.
    //從BLE Server取得所需要的服務
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    
    if (pRemoteService == nullptr) {
      Serial.print("Failed to find our service UUID: ");
      Serial.println(serviceUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    //找到該Service之UUID
    Serial.println(" - Found our service");


    // Obtain a reference to the characteristic in the service of the remote BLE server.
    //從BLE Service取得所需要的特徵
    pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID1);
    if (pRemoteCharacteristic == nullptr) {
      Serial.print("Failed to find our characteristic UUID: ");
      Serial.println(charUUID1.toString().c_str());
      pClient->disconnect();
      return false;
    }
    //找到該特徵之UUID
    Serial.println(" - Found our characteristic");

    // Read the value of the characteristic.
    // 由特徵讀取值
    if(pRemoteCharacteristic->canRead()) {
      std::string value = pRemoteCharacteristic->readValue();
      //Serial.print("The characteristic value was: ");
      //Serial.println(value.c_str());
    }


    if(pRemoteCharacteristic->canNotify())
      pRemoteCharacteristic->registerForNotify(notifyCallback);
    //連線成功
    connected = true;
    return true;
}
//回調BLE SERVER狀況
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
 /**
   * Called for each advertising BLE server.
   */
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print("BLE Advertised Device found: ");
    Serial.println(advertisedDevice.toString().c_str());

    // We have found a device, let us now see if it contains the service we are looking for.
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {
      //找到該SERVER，則停止廣播
      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan = true;

    } // Found our server
  } // onResult
}; // MyAdvertisedDeviceCallbacks

void setup() {
  Serial.begin(115200);
  Serial.println("開始BLE Client....");
  BLEDevice::init("esp_ble_Client");//初始化，並啟動BLE裝置
  
  //掃描附近BLE SERVER
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5, false);
}

void loop() {
  
  if (doConnect == true) {
    if (connectToServer()) {
      Serial.println("We are now connected to the BLE Server.");
    } else {
      Serial.println("We have failed to connect to the server; there is nothin more we will do.");
    }
    doConnect = false;
    }
    if (connected) {
      
    }
    else if(doScan){
      BLEDevice::getScan()->start(0);
    }
    
    delay(10);
}
