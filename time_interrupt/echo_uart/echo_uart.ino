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



void UART_Task(void *pvParam){
  
  uart_config_t uart2_config ={
    .baud_rate = 115200,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
  };

  uart_driver_install(UART_NUM_1, BUF_SIZE*2, 0, 0,NULL,0);
  uart_param_config(UART_NUM_1, &uart2_config);
  uart_set_pin(UART_NUM_1, TXD_PIN, RXD_PIN, UART_RTS, UART_CTS);
  
  uint8_t *data = (uint8_t *)malloc(BUF_SIZE);
  while(1){
    int len = uart_read_bytes(UART_NUM_1, data, BUF_SIZE ,20/portTICK_RATE_MS);
    uart_write_bytes(UART_NUM_1, (const char*)data,len);
  }
  
}
void setup() {
  
  xTaskCreate( UART_Task, "UART_Task", 1024, NULL, 10, NULL);

}

void loop() {
  

}
