#ifndef _QR_READER_H
#define _QR_READER_H

#include "stm32l4xx_hal.h"
#include <string.h>
#include <stdio.h>
#include "utility.h"
#include "wifi_module.h"

#define QR_SIZE 8
#define POTID_LEN 74

extern uint8_t qr_buf[QR_SIZE]; // buffer to hold 1 qr code
UART_HandleTypeDef *qr_huart; // huart for qr scanner

void init_qr_scanner(UART_HandleTypeDef* huart);
void send_qr_scan(uint8_t* qr_code);

#endif
