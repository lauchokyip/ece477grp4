// handles communication with the QR scanner over UART

#include "qr_scanner.h"

uint8_t qr_buf[QR_SIZE]; // buffer to hold qr scans
int qr_scan_pending; // if there is a qr scan waiting to be handled
UART_HandleTypeDef *qr_huart; // UART handle for QR scanner

/* 	call to set up first qr scan interrupt

	wifi=0 means use qr to set up, wifi=1 means no qr to set up
	if using qr scanner for wifi setup, call this before esp8266_init
	and then reset qr_scan_pending to 0 and call HAL_UART_Receive_IT 
	immediately after esp8266_init
*/
void qr_scanner_init(UART_HandleTypeDef* huart, int wifi) {
	qr_huart = huart;
	qr_scan_pending = 0;
	if (wifi != 0) {
		HAL_UART_Receive_IT(qr_huart, qr_buf, QR_SIZE);
	}
}

// handles a new qr scan
// to call, set a flag in RxComplete callback. 
// DO NOT CALL IN THE CALLBACK. Call from main loop instead.
void qr_scan_received(void) {
	// copy the qr code to a new array to prevent it from being overwritten by a new scan
	printf("Got QR scan: %s\r\n", qr_buf);
	int i;
	for (i=0; i<8; ++i) {
		printf("%d ", qr_buf[i]);
	}
	printf("\r\n");
	char qr_to_send[QR_SIZE];
	memcpy(qr_to_send, qr_buf, QR_SIZE);
	qr_scan_pending = 0;

	// add qr scan to wifi queue and reset buffer
	send_qr_scan(qr_to_send);
	memset(qr_buf, 0, QR_SIZE);
	HAL_UART_Receive_IT(qr_huart, qr_buf, QR_SIZE);
}

// sets up and enqueues get request with qr code
void send_qr_scan(char* qr_code) {
	uint8_t url[SCAN_URL_LEN + QR_SIZE-1];
	char url_str[SCAN_URL_LEN + QR_SIZE-1];
	sprintf(url_str, "https://virtualqueue477.herokuapp.com/barcodeScan?storeSecret=grp4&IDscanned=%s", qr_code);
	printf("url_str: %s\r\n\r\n", url_str);
	str_to_uint(url_str, url, SCAN_URL_LEN+QR_SIZE-1);
	printf("url: %s\r\n\r\n", url);
	new_message(1, url, SCAN_URL_LEN + QR_SIZE-1);
}
