#ifndef _QR_READER_H
#define _QR_READER_H

#include "stm32l4xx_hal.h"
#include <string.h>
#include <stdio.h>
#include "utility.h"
#include "wifi_module.h"

#define QR_SIZE 8
#define SCAN_URL_LEN 77

extern uint8_t qr_buf[QR_SIZE]; // buffer to hold 1 qr code
UART_HandleTypeDef *qr_huart; // huart for qr scanner
extern int qr_scan_pending;

void qr_scanner_init(UART_HandleTypeDef* huart);
void qr_scan_received(void);
void send_qr_scan(char* qr_code);

#endif
