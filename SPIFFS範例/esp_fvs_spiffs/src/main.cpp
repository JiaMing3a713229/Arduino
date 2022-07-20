#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_spiffs.h"
#include "dirent.h"

const char* ssid = "JiaMing";
const char* pass = "jia418009";
const char* ssid_root = "/spiffs/ssid.txt";
const char* pass_root = "/spiffs/pass.txt";

esp_vfs_spiffs_conf_t cong  = {
  .base_path = "/spiffs",
  .partition_label =  NULL,
  .max_files = 5,
  .format_if_mount_failed = true
};

void WriteDataFS(const char* RootLocation, const char* message){

  FILE* f = fopen(RootLocation, "w");
  if (f == NULL) {
      Serial.println("Failed to open file for writing");
        return;
    }
  fprintf(f, message);
  fprintf(f, "\n");
  fclose(f);
  Serial.println("File written");

}

String ReadDataFS(const char* root_read){
    FILE* f = fopen(root_read, "r");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for reading");
    }
    char line[64];
    fgets(line, sizeof(line), f);
    fclose(f);
    // strip newline
    char* pos = strchr(line, '\n');
    if (pos) {
        *pos = '\0';
    }
    //Serial.printf("Read from file: '%s'", line);
    return line;
}
void filelist(){

  DIR *d;
  d = opendir("/spiffs");

  struct dirent * entry;
  
  while ((entry = readdir(d))) {
    Serial.printf("%s\n",entry->d_name);
  }

  closedir(d);
}
void setup(){

  Serial.begin(115200);

  esp_err_t ret;
  ret = esp_vfs_spiffs_register(&cong);
  
  if(ret != ESP_OK){
    Serial.println("SPIFFS文件系統掛載失敗");
    if(ret == ESP_ERR_NOT_FOUND){
      Serial.println("esp32 設置分區表未設置SPIFFS起始位置!");
    }
  }
  Serial.println("SPIFFS文件系統掛載成功");
  delay(1000);

  filelist();

  size_t total,used;
  ret = esp_spiffs_info(NULL, &total, &used);
  if(ret !=ESP_OK){
    Serial.println("esp32 SPIFFS起始位置錯誤");
  }
  Serial.printf("Partition size: total: %d, used: %d\n", total, used);
  delay(1000);

  WriteDataFS(ssid_root, ssid);
  WriteDataFS(pass_root, pass);
  String read_data_ssid = ReadDataFS(ssid_root);
  String read_data_pass = ReadDataFS(pass_root);
  Serial.printf("%s內容:%s\n", ssid_root, read_data_ssid);
  Serial.printf("%s內容:%s\n", pass_root, read_data_pass);
  delay(1000);
  
  
  
}

void loop(){
   
}
