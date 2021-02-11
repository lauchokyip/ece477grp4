#ifndef __JSON_H
#define __JSON_H

#include "mytype.h"

/**
 * customer_t holds all the fields that a customer has
 */
typedef struct customer_t {
    int id;
    int numOfCustomer;
    bool isTimeToCheckIn;
    bool isNewCustomer;
    bool isOnQueue;
    int queueNum;
} customer_t;

/**
 * from_server_msg will have all the field needed and can be filled up from the server JSON 
 */
typedef struct from_server_msg{
    char status[8];
    bool isToMicroController;
    customer_t customer;
    char errMsg[128];
} from_server_msg;

int fill_in_struct_from_json(char *json_msg, from_server_msg* new_server_msg);

#endif