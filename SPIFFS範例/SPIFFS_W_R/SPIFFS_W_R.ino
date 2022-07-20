#include "SPIFFS.h"

const char* ssid = "ssid_fs";//想要儲存至SPIFFS中的網路名稱
const char* pass = "pass_fs";//想要儲存至SPIFFS中的網路密碼

const char* ssid_fs = "/ssid.txt";
const char* pass_fs = "/pass.txt";

void scanFile(){
  File root = SPIFFS.open("/");
  File file = root.openNextFile();
  while(file){
    Serial.printf("file:%s\n",file.name());
    file = root.openNextFile();
    
  }
}

void writeFile(const char* fileLocation, const char* write_message){

  File file = SPIFFS.open(fileLocation, "w"); // open and write to file
  if(!file){
    Serial.println("寫入失敗");
    return;
  }
  file.println(write_message);
  file.close();
  Serial.printf("write down %s\n",write_message);
  
}
String readFile(const char* fileLocation){
  
  String readFsMessage = "";
  File file = SPIFFS.open(fileLocation,"r"); //"r" open and read a file
  if(!file){
    Serial.println("開啟檔案失敗");
    return readFsMessage;
  }
  //Serial.printf("檔案大小:%u Bytes\n", file.size());
  //Serial.printf("file.available():%d\n",file.available());
  readFsMessage = file.readStringUntil('\n');
  /*
  while(file.available()){
    readFsMessage = file.readStringUntil('\n');
  }
  */
  file.close();
  return readFsMessage;
  
}


void init_SPIFFS(){
  if(!SPIFFS.begin(true)){
    Serial.println("SPIFFS 掛載失敗!");
  }
  Serial.println("SPIFFS 掛載成功!");
}
void setup() {

  Serial.begin(115200);

  init_SPIFFS();
  delay(1000);
  writeFile(ssid_fs,ssid);
  delay(1000);
  Serial.print("讀取內容:");
  Serial.println(readFile(ssid_fs));
  scanFile();
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:

}
