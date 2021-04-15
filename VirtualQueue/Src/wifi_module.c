// handles communication with the web server via the ESP-01 over UART

/* PUT THE FOLLOWING IN THE INTERRUPT HANDLER (not callback the real one in stm32l4xx_it.c) to deal with IDLE
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

#include "wifi_module.h"

uint8_t esp_recv_buf[2000]; // giant buffer for receiving messages
int wait_for_send_ok; // if waiting for the ok to send data from the ESP
int wait_for_message_response; // if waiting for a response from the web server
int good_for_send; // if ok to send data to ESP
int ready_for_next_message; // if ready to start from beginning with new message
int message_pending_handling; // if a response from the server needs handling
// below is information for the display, so it can be updated externally (ex: error messages) w/o having to get status
int queue_length; // length of queue
int store_capacity; // store capcity
int num_in_store; // number of people in store

UART_HandleTypeDef *esp_huart; // UART handle to ESP

/* 	set up the module and connects to internet
	currently uses blocking/polling so is dumb but is during setup so not the end of the world
	INPUT: 	huart = UART handle for ESP
			display = SPI handle for display
			wifi = 0 to skip wifi connection (useful for repeated testing as ESP remembers)
				   1 to use QR codes to set up
				   2 to use Nate's default testing hotspot
			fast = 0 to do full set up
				   1 to skip reset, mode set, and number of connections (useful for repeated testing as ESP remembers)
// TODO: add set up verification checks
*/
void esp8266_init(UART_HandleTypeDef* huart, SPI_HandleTypeDef* display, int wifi, int fast) {
	esp_huart = huart;
	display_handle = display;
	wait_for_send_ok = 0;
	wait_for_message_response = 0;
	good_for_send = 0;
	ready_for_next_message = 1;
	message_pending_handling = 0;
	message_queue_head = NULL;
	queue_length = 0;
	store_capacity = 0;
	num_in_store = 0;

	// reset and set basic params
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
		if (strstr(esp_recv_buf, "OK") == NULL) {
			main_display_info(display_handle, num_in_store, queue_length, store_capacity, "AN ERROR HAS OCCURRED", "ERROR: WIFI MODE", NULL, NULL);
			return;
		}
		printf("%s\r\n", esp_recv_buf);
		memset(esp_recv_buf, 0, 2000);

		// set connections to 1 at a time
		printf("set connections...\r\n");
		uint8_t numcons[] = "AT+CIPMUX=0\r\n";
		HAL_UART_Transmit(esp_huart, numcons, sizeof(numcons)/sizeof(uint8_t), 100);
		HAL_UART_Receive(esp_huart, esp_recv_buf, 2000, 500);
		if (strstr(esp_recv_buf, "OK") == NULL) {
			main_display_info(display_handle, num_in_store, queue_length, store_capacity, "AN ERROR HAS OCCURRED", "ERROR: WIFI NUMCONS", NULL, NULL);
			return;
		}
		printf("%s\r\n", esp_recv_buf);
		memset(esp_recv_buf, 0, 2000);
	}

	// connect to given wifi
	if (wifi != 0) {
	  printf("connect to wifi...\r\n");
	  if (wifi == 2) { // use nate's hotspot (testing only)
		  uint8_t connect[] = "AT+CWJAP=\"TEST-HOTSPOT\",\"65c9O21=\"\r\n";
		  HAL_UART_Transmit(esp_huart, connect, sizeof(connect)/sizeof(uint8_t), 100);
		  HAL_UART_Receive(esp_huart, esp_recv_buf, 2000, 10000);
		  printf("%s\r\n", esp_recv_buf);
		  memset(esp_recv_buf, 0, 2000);
	  } else { // using qr
		  uint8_t wifi_name[100];
		  memset(wifi_name, 0, 100);
		  printf("Please scan QR code for WiFi name:\r\n");
		  main_display_info(display_handle, num_in_store, queue_length, store_capacity, "Scan WiFi name...", NULL, NULL, NULL);
		  while (wifi_name[0] == 0) {
			  HAL_UART_Receive(qr_huart, wifi_name, 100, 1000);
		  }
		  printf("%s\r\n", wifi_name);
		  wifi_name[strlen(wifi_name)-1] = 0; // remove ending newline

		  uint8_t wifi_pass[100];
		  memset(wifi_pass, 0, 100);
		  printf("Please scan QR code for WiFi password:\r\n");
		  main_display_info(display_handle, num_in_store, queue_length, store_capacity, "Scan WiFi password...", NULL, NULL, NULL);
		  while (wifi_pass[0] == 0) {
			  HAL_UART_Receive(qr_huart, wifi_pass, 100, 1000);
		  }
		  printf("%s\r\n", wifi_pass);
		  wifi_pass[strlen(wifi_pass)-1] = 0; // remove ending newline
		  main_display_info(display_handle, num_in_store, queue_length, store_capacity, "Starting up...", "Please wait", NULL, NULL);

		  int c_size = strlen(wifi_name) + strlen(wifi_pass) + 16;
		  uint8_t connect[c_size];
		  char connect_str[c_size];
		  sprintf(connect_str, "AT+CWJAP=\"%s\",\"%s\"\r\n", wifi_name, wifi_pass);
		  printf("Connection command: %s\r\n", connect_str);
		  str_to_uint(connect_str, connect, 216);
		  HAL_UART_Transmit(esp_huart, connect, sizeof(connect)/sizeof(uint8_t), 100);
		  int count = 0;
		  while (strstr(esp_recv_buf, "WIFI GOT IP") == NULL) {
			  HAL_UART_Receive(esp_huart, esp_recv_buf, 2000, 5000);
			  ++count;
			  if (count > 10) {
					main_display_info(display_handle, num_in_store, queue_length, store_capacity, "AN ERROR HAS OCCURRED", "ERROR: WIFI CONNECT", NULL, NULL);
				  	return;
			  }
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
	if (strstr(esp_recv_buf, "OK") == NULL) {
		main_display_info(display_handle, num_in_store, queue_length, store_capacity, "AN ERROR HAS OCCURRED", "ERROR: WIFI TCP", NULL, NULL);
		return;
	}
	printf("%s\r\n", esp_recv_buf);
	memset(esp_recv_buf, 0, 2000);

	__HAL_UART_ENABLE_IT(esp_huart, UART_IT_IDLE); // enable IDLE line detection as message length is variable
	printf("ESP8266 INIT COMPLETE\r\n");
}

// adds new message to message queue
// NOTE: pass url_len = actual size - 1
// ^ this may be fixed later
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

// gets ok from ESP to send over data (GET request)
void get_ok_to_send() {
	ready_for_next_message = 0;
	wait_for_send_ok = 1;
	WifiMessage *m = message_queue_head;
	int digits = count_digits(m->url_len + GET_LEN);

	// create command
	uint8_t send_cmd[digits + SEND_CMD_LEN];
	char send_cmd_str[digits + SEND_CMD_LEN];
	sprintf(send_cmd_str, "AT+CIPSEND=%d\r\n", m->url_len + GET_LEN);
	str_to_uint(send_cmd_str, send_cmd, digits + SEND_CMD_LEN);

	// send
	printf("asking to send...\r\n");
	memset(esp_recv_buf, 0, 2000);
	HAL_UART_Transmit(esp_huart, send_cmd, sizeof(send_cmd)/sizeof(uint8_t), 100);
	HAL_UART_Receive_DMA(esp_huart, esp_recv_buf, 2000);
}

// have gotten ok to send, so now send the data
void send_message() {
	printf("OK TO SEND:\r\n");
	printf("%s\r\n", esp_recv_buf);
	good_for_send = 0;
	wait_for_message_response = 1;
	WifiMessage *m = message_queue_head;

	// create "string" to send
	uint8_t data[m->url_len + GET_LEN];
	char data_str[m->url_len + GET_LEN];
	sprintf(data_str, "GET %s HTTP/1.1\r\nHost: virtualqueue477.herokuapp.com\r\nConnection: keep-alive\r\n\r\n", m->url);
	str_to_uint(data_str, data, m->url_len + GET_LEN);

	// send
	printf("sending...\r\n");
	printf("To send:\r\n%s\r\n", data_str);
	memset(esp_recv_buf, 0, 2000);
	HAL_UART_Transmit(esp_huart, data, sizeof(data)/sizeof(uint8_t), 100);
	HAL_UART_Receive_DMA(esp_huart, esp_recv_buf, 2000);
}

// have gotten a reponse from the web server
// determine the type and handle appropriately
void handle_message_response() {
	printf("Response: %s\r\n", esp_recv_buf);
	WifiMessage *m = message_queue_head;

	// if not ok, abandon
	if (strstr(esp_recv_buf, "HTTP/1.1 200 OK") == NULL) {
		printf("HTTP ERROR\r\n");
		char error[19];
		sprintf(error, "ERROR: HTTP FAIL %d", m->type);
		main_display_info(display_handle, num_in_store, queue_length, store_capacity, "AN ERROR HAS OCCURRED", error, NULL, NULL);
		message_queue_head = message_queue_head->next;
		free(m->url);
		free(m);
		message_pending_handling = 0;
		ready_for_next_message = 1;
		return;
	}

	// get JSON out of repsonse
	char* start = index(esp_recv_buf, '{');
	char* end = rindex(esp_recv_buf, '}');
	int json_len = end-start + 1;
	printf("JSON length: %d\r\n", json_len);
	if (json_len <= 0) {
		printf("ERROR: JSON NOT FOUND IN STRING\r\n");
		char error[18];
		sprintf(error, "ERROR: JSON DNE %d", m->type);
		main_display_info(display_handle, num_in_store, queue_length, store_capacity, "AN ERROR HAS OCCURRED", error, NULL, NULL);
		message_queue_head = message_queue_head->next;
		free(m->url);
		free(m);
		message_pending_handling = 0;
		ready_for_next_message = 1;
		return;
	}
	char json[json_len+1];
	memcpy(json, start, json_len);
	json[json_len] = 0;
	printf("JSON string: %s\r\n", json);

	// determine type
	if (m->type == 1) { // QR scan
		printf("WAS QR SCAN\r\n");
		barcode_server_msg* parsed_message = barcode_parse_json(esp_recv_buf);
		if (parsed_message == NULL) {
			printf("FAILED TO PARSE JSON\r\n");
			char error[19];
			sprintf(error, "ERROR: JSON FAIL %d", m->type);
			main_display_info(display_handle, num_in_store, queue_length, store_capacity, "AN ERROR HAS OCCURRED", error, NULL, NULL);
		} else {
			print_out_barcode_msg(parsed_message);
			// determine action - nothing or begin temp
			free(parsed_message);
		}
	} else if (m->type == 2) { // things w/o data - entry, exit, tempError, unauthEntry
		printf("WAS NO DATA TYPE (ENTRY, EXIT, TEMP ERROR, UNAUTH ENTRY)\r\n");
		no_data_server_msg* parsed_message = no_data_parse_json(esp_recv_buf);
		if (parsed_message == NULL) {
			printf("FAILED TO PARSE JSON\r\n");
			char error[19];
			sprintf(error, "ERROR: JSON FAIL %d", m->type);
			main_display_info(display_handle, num_in_store, queue_length, store_capacity, "AN ERROR HAS OCCURRED", error, NULL, NULL);
		} else {
			print_out_no_data_msg(parsed_message);
			free(parsed_message);
		}
	} else if (m->type == 3) { // status for display
		printf("WAS STATUS\r\n");
		status_server_msg* parsed_message = status_parse_json(esp_recv_buf);
		if (parsed_message == NULL) {
			printf("FAILED TO PARSE JSON\r\n");
			char error[19];
			sprintf(error, "ERROR: JSON FAIL %d", m->type);
			main_display_info(display_handle, num_in_store, queue_length, store_capacity, "AN ERROR HAS OCCURRED", error, NULL, NULL);
		} else {
			print_out_status_msg(parsed_message);
			queue_length = parsed_message->queueLength;
			num_in_store = parsed_message->numPeopleInStore;
			store_capacity = parsed_message->maxCapacity;
			main_display_info(display_handle, num_in_store, queue_length, store_capacity, "     Welcome to ABC store!", NULL, NULL, NULL);
			free(parsed_message);
		}
	}

	// remove message from queue
	message_queue_head = message_queue_head->next;
	free(m->url);
	free(m);
	message_pending_handling = 0;
	ready_for_next_message = 1;
	printf("DONE HANDLING RESPONSE\r\n");
}

// enqueues an entry message
void send_entry() {
	uint8_t url[] = "https://virtualqueue477.herokuapp.com/enteredStore?storeSecret=grp4";
	new_message(2, url, sizeof(url)/sizeof(uint8_t)-1);
}

// enqueues an exit message
void send_exit() {
	uint8_t url[] = "https://virtualqueue477.herokuapp.com/leftStore?storeSecret=grp4";
	new_message(2, url, sizeof(url)/sizeof(uint8_t)-1);
}

// enqueues a status message
void get_status() {
	uint8_t url[] = "https://virtualqueue477.herokuapp.com/getStatus?storeSecret=grp4";
	new_message(3, url, sizeof(url)/sizeof(uint8_t)-1);
}

