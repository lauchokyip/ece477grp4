#include "qr_scanner.h"

uint8_t qr_buf[QR_SIZE];
int qr_scan_pending;

// call to set up first qr scan interrupt
// pass the UART_HandleTypeDef of the uart channel the qr scanner uses
void qr_scanner_init(UART_HandleTypeDef* huart) {
	qr_huart = huart;
	qr_scan_pending = 0;
	HAL_UART_Receive_IT(qr_huart, qr_buf, QR_SIZE);
}

// call this function in the corresponding HAL_UART_RxCpltCallback branch
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
	send_qr_scan(qr_to_send); // TODO: send qr scan to server using wifi
	--qr_scan_pending;
	BSP_LCD_GLASS_DisplayString("DONE");
	HAL_UART_Receive_IT(qr_huart, qr_buf, QR_SIZE);
}

// sets up and sends get request with qr code
void send_qr_scan(char* qr_code) {
	uint8_t url[SCAN_URL_LEN + QR_SIZE];
	char url_str[SCAN_URL_LEN + QR_SIZE];
	sprintf(url_str, "https://virtualqueue477.herokuapp.com/barcodeScan?storeSecret=grp4&IDscanned=%s", qr_code);
	printf("url_str: %s\r\n\r\n", url_str);
	str_to_uint(url_str, url, SCAN_URL_LEN+QR_SIZE);
	printf("url: %s\r\n\r\n", url);
	BSP_LCD_GLASS_DisplayString("WIFI");
	send_get(url, SCAN_URL_LEN + QR_SIZE);
}
