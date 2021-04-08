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
void esp8266_init(UART_HandleTypeDef* huart, int wifi, int fast) {
	esp_huart = huart;
	wait_for_send_ok = 0;
	wait_for_message_response = 0;
	good_for_send = 0;
	ready_for_next_message = 1;
	message_pending_handling = 0;
	message_queue_head = NULL;

	// reset
	if (fast != 1) {
		printf("reset...\r\n");
		uint8_t reset[] = "AT+RST\r\n";
		HAL_UART_Transmit(esp_huart, reset, sizeof(reset)/sizeof(uint8_t), 100);
		HAL_UART_Receive(esp_huart, esp_recv_buf, 2000, 5000);
		printf("%s\r\n", esp_recv_buf);
		memset(esp_recv_buf, 0, 2000);

		// set mode to station/client
		printf("set mode...\r\n");
		uint8_t mode[] = "AT+CWMODE=1\r\n";
		HAL_UART_Transmit(esp_huart, mode, sizeof(mode)/sizeof(uint8_t), 100);
		HAL_UART_Receive(esp_huart, esp_recv_buf, 2000, 500);
		printf("%s\r\n", esp_recv_buf);
		memset(esp_recv_buf, 0, 2000);

		// set connections to 1 at a time
		printf("set connections...\r\n");
		uint8_t numcons[] = "AT+CIPMUX=0\r\n";
		HAL_UART_Transmit(esp_huart, numcons, sizeof(numcons)/sizeof(uint8_t), 100);
		HAL_UART_Receive(esp_huart, esp_recv_buf, 2000, 500);
		printf("%s\r\n", esp_recv_buf);
		memset(esp_recv_buf, 0, 2000);
	}

	// connect to given wifi
	if (wifi != 0) {
	  printf("connect to wifi...\r\n");
	  if (wifi == 2) {
		  uint8_t connect[] = "AT+CWJAP=\"TEST-HOTSPOT\",\"65c9O21=\"\r\n";
		  HAL_UART_Transmit(esp_huart, connect, sizeof(connect)/sizeof(uint8_t), 100);
		  HAL_UART_Receive(esp_huart, esp_recv_buf, 2000, 10000);
		  printf("%s\r\n", esp_recv_buf);
		  memset(esp_recv_buf, 0, 2000);
	  } else {
		  uint8_t wifi_name[100];
		  memset(wifi_name, 0, 100);
		  printf("Please scan QR code for WiFi name:\r\n");
		  while (wifi_name[0] == 0) {
			  HAL_UART_Receive(qr_huart, wifi_name, 100, 1000);
		  }
		  printf("%s\r\n", wifi_name);
		  wifi_name[strlen(wifi_name)-1] = 0; // remove ending newline

		  uint8_t wifi_pass[100];
		  memset(wifi_pass, 0, 100);
		  printf("Please scan QR code for WiFi password:\r\n");
		  while (wifi_pass[0] == 0) {
			  HAL_UART_Receive(qr_huart, wifi_pass, 100, 1000);
		  }
		  printf("%s\r\n", wifi_pass);
		  wifi_pass[strlen(wifi_pass)-1] = 0; // remove ending newline

		  int c_size = strlen(wifi_name) + strlen(wifi_pass) + 16;
		  uint8_t connect[c_size];
		  char connect_str[c_size];
		  sprintf(connect_str, "AT+CWJAP=\"%s\",\"%s\"\r\n", wifi_name, wifi_pass);
		  printf("Connection command: %s\r\n", connect_str);
		  str_to_uint(connect_str, connect, 216);
		  HAL_UART_Transmit(esp_huart, connect, sizeof(connect)/sizeof(uint8_t), 100);
		  while (strstr(esp_recv_buf, "WIFI GOT IP") == NULL) {
			  HAL_UART_Receive(esp_huart, esp_recv_buf, 2000, 5000);
		  }
		  printf("%s\r\n", esp_recv_buf);
		  memset(esp_recv_buf, 0, 2000);
	  }
	}

	// start tcp connection to server
	printf("connect to VQ web server...\r\n");
	uint8_t start[] = "AT+CIPSTART=\"TCP\",\"virtualqueue477.herokuapp.com\",80\r\n";
	HAL_UART_Transmit(esp_huart, start, sizeof(start)/sizeof(uint8_t), 100);
	HAL_UART_Receive(esp_huart, esp_recv_buf, 2000, 5000);
	printf("%s\r\n", esp_recv_buf);
	memset(esp_recv_buf, 0, 2000);
	__HAL_UART_ENABLE_IT(esp_huart, UART_IT_IDLE); // enable IDLE line detection as message length is variable
	printf("ESP8266 INIT COMPLETE\r\n");
}

void new_message(int type, uint8_t* url, int url_len) {
	printf("INSERTING NEW MESSAGE\r\n");
	WifiMessage *m = malloc(sizeof(WifiMessage));
	m->type = type;
	m->url = malloc(url_len+1);
	memcpy(m->url, url, url_len);
	m->url[url_len] = '\0';
	m->url_len = url_len;
	m->next = NULL;
	if (message_queue_head == NULL) {
		printf("INSERTED AS HEAD\r\n");
		message_queue_head = m;
	} else {
		WifiMessage *tmp = message_queue_head;
		while (tmp->next != NULL) {
			tmp = tmp->next;
		}
		tmp->next = m;
	}
	printf("NEW MESSAGE INSERTED\r\n");
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
	memset(esp_recv_buf, 0, 2000);
	HAL_UART_Transmit(esp_huart, send_cmd, sizeof(send_cmd)/sizeof(uint8_t), 100);
	HAL_UART_Receive_DMA(esp_huart, esp_recv_buf, 2000);
}

void send_message() {
	printf("OK TO SEND:\r\n");
	printf("%s\r\n", esp_recv_buf);
	good_for_send = 0;
	wait_for_message_response = 1;
	WifiMessage *m = message_queue_head;

	uint8_t data[m->url_len + GET_LEN];
	char data_str[m->url_len + GET_LEN];
	sprintf(data_str, "GET %s HTTP/1.1\r\nHost: virtualqueue477.herokuapp.com\r\nConnection: keep-alive\r\n\r\n", m->url);
	str_to_uint(data_str, data, m->url_len + GET_LEN);

	printf("sending...\r\n");
	printf("To send:\r\n%s\r\n", data_str);
	BSP_LCD_GLASS_DisplayString("SEND");
	memset(esp_recv_buf, 0, 2000);
	HAL_UART_Transmit(esp_huart, data, sizeof(data)/sizeof(uint8_t), 100);
	HAL_UART_Receive_DMA(esp_huart, esp_recv_buf, 2000);
}

void handle_message_response() {
	printf("Response: %s\r\n", esp_recv_buf);
	WifiMessage *m = message_queue_head;
	if (m->type == 1) { // QR scan
		printf("WAS QR SCAN\r\n");
		// parse QR JSON
		// determine action - nothing or begin temp
	} else if (m->type == 2) { // things w/o data - exit, tempError, unauthEntry
		printf("WAS NO DATA TYPE (EXIT, TEMP ERROR, UNAUTH ENTRY)\r\n");
	} else if (m->type == 3) { // entry
		// determine if allowed
		printf("WAS ENTRY\r\n");
	} else if (m->type == 4) { // status
		// parse status JSON
		// send to display
		printf("WAS STATUS\r\n");
	}
	message_queue_head = message_queue_head->next;
	free(m->url);
	free(m);
	message_pending_handling = 0;
	ready_for_next_message = 1;
	printf("DONE HANDLING RESPONSE\r\n");
}

void send_entry() {
	uint8_t url[] = "https://virtualqueue477.herokuapp.com/enteredStore?storeSecret=grp4";
	new_message(3, url, sizeof(url)/sizeof(uint8_t)-1);
}

void send_exit() {
	uint8_t url[] = "https://virtualqueue477.herokuapp.com/leftStore?storeSecret=grp4";
	new_message(2, url, sizeof(url)/sizeof(uint8_t)-1);
}


/* PUT THE FOLLOWING IN THE INTERRUPT HANDLER (not callback the real one) to deal with IDLE
  if(RESET != __HAL_UART_GET_FLAG(&huart4, UART_FLAG_IDLE)) { //Judging whether it is idle interruption
  	__HAL_UART_CLEAR_IDLEFLAG(&huart4);
  	HAL_UART_DMAStop(&huart4);
  	//uint8_t data_length  = 2000 - __HAL_DMA_GET_COUNTER(&hdma_uart4_rx);
  	if (wait_for_send_ok == 1) {
  		wait_for_send_ok = 0;
  		good_for_send = 1;
  	} else if (wait_for_message_response == 1) {
  		wait_for_message_response = 0;
  		message_pending_handling = 1;
  	}

  }
*/