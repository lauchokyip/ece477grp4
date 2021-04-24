#ifndef __JSON_H
#define __JSON_H

#include "mytype.h"

#define JSON_ITEM_MAX_SIZE 70

// Holds customer information
typedef struct customer_t {
    char name[JSON_ITEM_MAX_SIZE];
    char email[JSON_ITEM_MAX_SIZE];
    char id[8];
    int numPeople;
} customer_t;

// Response structure for a barcode scan
typedef struct barcode_server_msg {
    bool ok;
    customer_t customer;
    bool isCheckingIn;
} barcode_server_msg;

// Response structure for a message that returns no data
typedef struct no_data_server_msg {
	bool ok;
} no_data_server_msg;

// Response structure for queue status
typedef struct  {
	bool ok;
	int queueLength;
	int numPeopleInStore;
	int maxCapacity;
} status_server_msg;

barcode_server_msg* barcode_parse_json(char *json_msg);
no_data_server_msg* no_data_parse_json(char *json_msg);
status_server_msg* status_parse_json(char *json_msg);
void print_out_barcode_msg(barcode_server_msg*);
void print_out_no_data_msg(no_data_server_msg*);
void print_out_status_msg(status_server_msg*);

#endif
