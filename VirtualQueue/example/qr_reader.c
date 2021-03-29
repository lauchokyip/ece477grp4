#define QR_SIZE 8
#define POTID_LEN 74

uint8_t qr_buf[QR_SIZE]; // buffer to hold 1 qr code
UART_HandleTypeDef qr_huart; // huart for qr scanner


// call to set up first qr scan interrupt
// pass the UART_HandleTypeDef of the uart channel the qr scanner uses
void init_qr_scanner(UART_HandleTypeDef* huart) {
	qr_huart = *huart;
	HAL_UART_Receive_IT(&qr_huart, qr_buf, QR_SIZE);
}

// call this function in the corresponding HAL_UART_RxCpltCallback branch
void qr_scan_received(void) {
	// copy the qr code to a new array to prevent it from being overwritten by a new scan
	uint8_t qr_to_send[QR_SIZE];
	memcpy(qr_to_send, qr_buf, QR_SIZE);
	send_qr_scan(qr_to_send); // TODO: send qr scan to server using wifi
	HAL_UART_Receive_IT(&qr_huart, qr_buf, QR_SIZE);
}

// sets up and sends get request with qr code
void send_qr_scan(uint8_t* qr_code) {
	uint8_t url[POTID_LEN + QR_SIZE];
	sprintf(url, "https://virtualqueue477.herokuapp.com/enterQueue?storeSecret=grp4&potenID=%s", qr_code);
	send_get(url);
}
