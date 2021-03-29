#ifndef _WIFI_MODULE_H
#define _WIFI_MODULE_H

#include "stm32l4xx_hal.h"
#include "utility.h"
#include <stdio.h>

#define SEND_CMD_LEN 13
#define GET_LEN 54

extern uint8_t esp_recv_buf[2000]; // giant buffer for receiving messages
UART_HandleTypeDef *esp_huart; // huart for ESP-01
extern int wait_to_send;

void esp8266_init(UART_HandleTypeDef*, int, int); // pass huart for esp, connection=0 for heroku, 1 for ptsv2, wifi=1 to connect to wifi
void send_get(uint8_t*, int);
void handle_esp_response(UART_HandleTypeDef);

#endif
