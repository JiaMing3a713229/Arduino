#include "driver/uart.h"

//UART1

#define TXD_PIN 17
#define RXD_PIN 16
#define UART_RTS UART_PIN_NO_CHANGE
#define UART_CTS UART_PIN_NO_CHANGE

#define RX2_BUF_SIZE        1024
#define TX2_BUF_SIZE        1024
#define TXD2_PIN            17
#define RXD2_PIN            16 

#define BUF_SIZE 1024
#define UART_MAX_NUM_RX_BYTES 1024

static void uartEventTask(void *pvParam);
static QueueHandle_t s_uart2Quene;
static const char *TAG = "board_uart";

void UART_Init(){
  uart_config_t uart2_config ={
    .baud_rate = 115200,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
  };
  uart_driver_install(UART_NUM_1, BUF_SIZE*2, BUF_SIZE*2, 100,&s_uart2Quene,0);
  uart_param_config(UART_NUM_1, &uart2_config);
  uart_set_pin(UART_NUM_1, TXD_PIN, RXD_PIN, UART_RTS, UART_CTS);
}

void uartEventTask(void *pvParam){
  
  uart_event_t event;
  uint8_t *pTempBuf = (uint8_t *)malloc(UART_MAX_NUM_RX_BYTES);
  while(1){
    if(xQueueReceive(s_uart2Quene,(void *)&event,(portTickType)portMAX_DELAY)){
      bzero(pTempBuf, UART_MAX_NUM_RX_BYTES);
      switch(event.type){
        case UART_DATA:
          uart_read_bytes(UART_NUM_1,pTempBuf,event.size,portMAX_DELAY);
          uart_write_bytes(UART_NUM_1,(const char *)pTempBuf,event.size);
          break;
        case UART_FIFO_OVF:
          ESP_LOGI(TAG, "hw fifo overflow");
          uart_flush_input(UART_NUM_1);
          xQueueReset(s_uart2Quene);
          break;
        case UART_BUFFER_FULL:
          ESP_LOGI(TAG, "ring buffer full");
          uart_flush_input(UART_NUM_1);
          xQueueReset(s_uart2Quene);
          break;
        case UART_PARITY_ERR:
          ESP_LOGI(TAG, "uart parity error");
          break;
        case UART_FRAME_ERR:
          ESP_LOGI(TAG, "uart frame error");
          break;
        default:
          ESP_LOGI(TAG, "uart event type:%d",event.type);
          break;
        
      }//switch
      Serial.println((const char *)pTempBuf);
      free(pTempBuf);
      pTempBuf = NULL;
      vTaskDelete(NULL);
    }
  }
  
}
void setup() {

  Serial.begin(115200);
  xTaskCreate( uartEventTask, "uartEventTask", 2048, NULL, 12, NULL);

}

void loop() {
  

}
