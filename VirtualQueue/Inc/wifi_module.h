#ifndef _WIFI_MODULE_H
#define _WIFI_MODULE_H

#include "stm32l4xx_hal.h"
#include "utility.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "qr_scanner.h"
#include "json.h"
#include "display.h"

#define SEND_CMD_LEN 13
#define GET_LEN 78

// Used to construct a queue of pending messages to send over WiFi
typedef struct WifiMessage {
	int type; // type of message: 1=QR scan, 2=entry,exit,tempError,unauthEntry, 4=getStatus
	uint8_t* url; // url of message to send
	int url_len; // length of url
	struct WifiMessage *next; // next message in queue
} WifiMessage;

extern uint8_t esp_recv_buf[2000];
extern int wait_for_send_ok;
extern int wait_for_message_response;
extern int good_for_send;
extern int ready_for_next_message;
extern int message_pending_handling;
extern int queue_length;
extern int store_capacity;
extern int num_in_store;
extern int valid_entries;
extern UART_HandleTypeDef *esp_huart;

WifiMessage* message_queue_head; // head of message queue
SPI_HandleTypeDef* display_handle;

bool esp8266_init(UART_HandleTypeDef*, SPI_HandleTypeDef*, int, int);
void new_message(int, uint8_t*, int);
void get_ok_to_send();
void send_message();
void handle_message_response();
void send_entry();
void send_exit();
void get_status();
void send_tempError();
void send_unauthorizedEntry();

#endif
