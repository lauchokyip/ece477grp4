#ifndef _WIFI_MODULE_H
#define _WIFI_MODULE_H

#include "stm32l4xx_hal.h"
#include "utility.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SEND_CMD_LEN 13
#define GET_LEN 54

typedef struct WifiMessage {
	int type; // type of message: 1=QR scan, 2=exit,tempError,unauthEntry, 3=entry, 4=getStatus
	uint8_t* url; // url of message to send
	int url_len; // length of url
	struct WifiMessage *next; // next message in queue
} WifiMessage;

extern uint8_t esp_recv_buf[2000]; // giant buffer for receiving messages
extern int wait_for_send_ok;
extern int wait_for_message_response;
extern int good_for_send;
extern int ready_for_next_message;
extern int message_pending_handling;
extern UART_HandleTypeDef *esp_huart; // huart for ESP-01

WifiMessage* message_queue_head;


void esp8266_init(UART_HandleTypeDef*, int, int); // pass huart for esp, connection=0 for heroku, 1 for ptsv2, wifi=1 to connect to wifi
void send_get(uint8_t*, int);
void handle_esp_response(UART_HandleTypeDef);
void new_message(int, uint8_t*, int);
void get_ok_to_send();
void send_message();
void handle_message_response();

#endif
