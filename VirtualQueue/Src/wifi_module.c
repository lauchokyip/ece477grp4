#include "wifi_module.h"

uint8_t esp_recv_buf[2000]; // giant buffer for receiving messages
int wait_for_send_ok;
int wait_for_message_response;
int good_for_send;
int ready_for_next_message;
int message_pending_handling;
UART_HandleTypeDef *esp_huart;

// set up the module and connect to internet
// currently uses blocking/polling so is dumb but is during setup so not the end of the world
// pass huart for esp, connection=0 for heroku, 1 for ptsv2
// TODO: set up to take in wifi name and password as params
// TODO: add set up verification checks
void esp8266_init(UART_HandleTypeDef* huart, int connection, int wifi) {
	esp_huart = huart;
	__HAL_UART_ENABLE_IT(esp_huart, UART_IT_IDLE); // enable IDLE line detection as message length is variable
	wait_for_send_ok = 0;
	wait_for_message_response = 0;
	good_for_send = 0;
	ready_for_next_message = 1;
	message_pending_handling = 0;
	message_queue_head = NULL;

	// reset
	printf("reset...\r\n");
	uint8_t reset[] = "AT+RST\r\n";
	HAL_UART_Transmit(esp_huart, reset, sizeof(reset)/sizeof(uint8_t), 100);
	HAL_UART_Receive(esp_huart, esp_recv_buf, 2000, 5000);
	printf("%s\r\n", esp_recv_buf);
	clear_buf(esp_recv_buf, 2000);

	// set mode to station/client
	printf("set mode...\r\n");
	uint8_t mode[] = "AT+CWMODE=1\r\n";
	HAL_UART_Transmit(esp_huart, mode, sizeof(mode)/sizeof(uint8_t), 100);
	HAL_UART_Receive(esp_huart, esp_recv_buf, 2000, 500);
	printf("%s\r\n", esp_recv_buf);
	clear_buf(esp_recv_buf, 2000);

	// set connections to 1 at a time
	printf("set connections...\r\n");
	uint8_t numcons[] = "AT+CIPMUX=0\r\n";
	HAL_UART_Transmit(esp_huart, numcons, sizeof(numcons)/sizeof(uint8_t), 100);
	HAL_UART_Receive(esp_huart, esp_recv_buf, 2000, 500);
	printf("%s\r\n", esp_recv_buf);
	clear_buf(esp_recv_buf, 2000);

	// connect to given wifi
	if (wifi == 1) {
	  //printf("connect to wifi...\r\n");
	  uint8_t connect[] = "AT+CWJAP=\"TEST-HOTSPOT\",\"65c9O21=\"\r\n";
	  HAL_UART_Transmit(esp_huart, connect, sizeof(connect)/sizeof(uint8_t), 100);
	  HAL_UART_Receive(esp_huart, esp_recv_buf, 2000, 10000);
	  //printf("%s\r\n", esp_recv_buf);
	  clear_buf(esp_recv_buf, 2000);
	}

	// start tcp connection to server
	if (connection == 0) {
		printf("connect to VQ web server...\r\n");
		uint8_t start[] = "AT+CIPSTART=\"TCP\",\"virtualqueue477.herokuapp.com\",80\r\n";
		HAL_UART_Transmit(esp_huart, start, sizeof(start)/sizeof(uint8_t), 100);
		HAL_UART_Receive(esp_huart, esp_recv_buf, 2000, 5000);
	} else if (connection == 1) {
		//printf("connect to ptsv2...\r\n");
		uint8_t start[] = "AT+CIPSTART=\"TCP\",\"www.ptsv2.com\",80\r\n";
		HAL_UART_Transmit(esp_huart, start, sizeof(start)/sizeof(uint8_t), 100);
		HAL_UART_Receive(esp_huart, esp_recv_buf, 2000, 5000);
	}
	printf("%s\r\n", esp_recv_buf);
	clear_buf(esp_recv_buf, 2000);
	printf("ESP8266 INIT COMPLETE\r\n");
}

// sends a get request to the given url
void send_get(uint8_t* url, int url_len) {
	int digits = count_digits(url_len + GET_LEN);
	//printf("DIGITS=%d\r\n", digits);

	uint8_t send_cmd[digits + SEND_CMD_LEN];
	char send_cmd_str[digits + SEND_CMD_LEN];
	sprintf(send_cmd_str, "AT+CIPSEND=%d\r\n", url_len + GET_LEN);
	str_to_uint(send_cmd_str, send_cmd, digits + SEND_CMD_LEN);

	uint8_t data[url_len + GET_LEN];
	char data_str[url_len + GET_LEN];
	sprintf(data_str, "GET %s HTTP/1.1\r\nHost: virtualqueue477.herokuapp.com\r\n\r\n", url);
	str_to_uint(data_str, data, url_len + GET_LEN);

	printf("asking to send...\r\n");
	BSP_LCD_GLASS_DisplayString("ASK ");
	HAL_UART_Transmit(esp_huart, send_cmd, sizeof(send_cmd)/sizeof(uint8_t), 100);
	HAL_UART_Receive(esp_huart, esp_recv_buf, 2000, 5000);
	printf("%s\r\n", esp_recv_buf);
    clear_buf(esp_recv_buf, 2000);
    printf("sending...\r\n");
    BSP_LCD_GLASS_DisplayString("SEND");
    HAL_UART_Transmit(esp_huart, data, sizeof(data)/sizeof(uint8_t), 100);
    HAL_UART_Receive(esp_huart, esp_recv_buf, 2000, 5000);
    printf("%s\r\n", esp_recv_buf);
    clear_buf(esp_recv_buf, 2000);
    printf("GET sent\r\n");
}

void new_message(int type, uint8_t* url, int url_len) {
	WifiMessage *m = malloc(sizeof(WifiMessage));
	m->type = type;
	memcpy(m->url, url, url_len);
	m->url_len = url_len;
	if (message_queue_head == NULL) {
		message_queue_head = m;
	} else {
		WifiMessage *tmp = message_queue_head;
		while (tmp->next != NULL) {
			tmp = tmp->next;
		}
		tmp->next = m;
	}
}

void get_ok_to_send() {
	ready_for_next_message = 0;
	wait_for_send_ok = 1;
	WifiMessage *m = message_queue_head;
	int digits = count_digits(m->url_len + GET_LEN);
	//printf("DIGITS=%d\r\n", digits);

	uint8_t send_cmd[digits + SEND_CMD_LEN];
	char send_cmd_str[digits + SEND_CMD_LEN];
	sprintf(send_cmd_str, "AT+CIPSEND=%d\r\n", m->url_len + GET_LEN);
	str_to_uint(send_cmd_str, send_cmd, digits + SEND_CMD_LEN);

	printf("asking to send...\r\n");
	BSP_LCD_GLASS_DisplayString("ASK ");
	clear_buf(esp_recv_buf, 2000);
	HAL_UART_Transmit(esp_huart, send_cmd, sizeof(send_cmd)/sizeof(uint8_t), 100);
	HAL_UART_Receive_IT(esp_huart, esp_recv_buf, 2000);
}

void send_message() {
	good_for_send = 0;
	wait_for_message_response = 1;
	WifiMessage *m = message_queue_head;

	uint8_t data[m->url_len + GET_LEN];
	char data_str[m->url_len + GET_LEN];
	sprintf(data_str, "GET %s HTTP/1.1\r\nHost: virtualqueue477.herokuapp.com\r\n\r\n", m->url);
	str_to_uint(data_str, data, m->url_len + GET_LEN);

	printf("sending...\r\n");
	BSP_LCD_GLASS_DisplayString("SEND");
	clear_buf(esp_recv_buf, 2000);
	HAL_UART_Transmit(esp_huart, data, sizeof(data)/sizeof(uint8_t), 100);
	HAL_UART_Receive_IT(esp_huart, esp_recv_buf, 2000);
}

void handle_message_response() {
	WifiMessage *m = message_queue_head;

	if (m->type == 1) { // QR scan
		// parse QR JSON
		// determine action - nothing or begin temp
	} else if (m->type == 2) { // things w/o data - exit, tempError, unauthEntry

	} else if (m->type == 3) { // entry
		// determine if allowed
	} else if (m->type == 4) { // status
		// parse status JSON
		// send to display
	} else {
		// error
	}
	message_queue_head = message_queue_head->next;
	free(m);
	message_pending_handling = 0;
	ready_for_next_message = 1;
}




/* PUT THE FOLLOWING IN THE INTERRUPT HANDLER (not callback the real one) to deal with IDLE
  if(RESET != __HAL_UART_GET_FLAG(esp_huart, UART_FLAG_IDLE))   //Judging whether it is idle interruption
  {
	__HAL_UART_CLEAR_IDLEFLAG(esp_huart);                       //Clear idle interrupt sign (otherwise it will continue to enter interrupt)
	if (wait_for_send_ok == 1) {                  		//Call interrupt handler
		wait_for_send_ok = 0;
		good_for_send = 1;
	} else if (wait_for_message_response == 1) {
		wait_for_message_response = 0;
		message_pending_handling = 1;
	}
  }
*/
