// parses json
// to use properly, create a struct in json.h, then create a version of x_parse_json() for it

#include "json.h"
#include "jsmn.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) 
{
  if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
      strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
    return 0;
  }
  return -1;
}

void my_strcpy(char *src, char *dst, int num)
{
    int i;

    if(src == NULL || dst == NULL)
        return;
    for(i = 0; i < num; i++)
    {
        if(src[i] == '\"' || src[i] == ','|| src[i] == 0)
        {
            dst[i] = 0;
            return;
        }

        dst[i]=src[i];
    }
    dst[i-1] = 0;
}

int return_true_or_false_from_str(char *str)
{
    if(strncmp(str, "true",4) == 0)
        return true;
    if(strncmp(str, "false",5) == 0)
        return false;
    printf("%s\r\n",str);
    return -1;
}

// convert barcode scan response
barcode_server_msg* barcode_parse_json(char *json_msg)
{
    int i;
    int r;
    int ret;
    jsmn_parser p;
    jsmntok_t token[1024]; /* We expect no more than 1024 tokens */
    char temp_str[8];

    barcode_server_msg* new_msg = malloc(sizeof (barcode_server_msg));
    if (new_msg == NULL)
        return NULL;

    jsmn_init(&p);
    r = jsmn_parse(&p, json_msg, strlen(json_msg), token,
                  sizeof(token) / sizeof(token[0]));
    if (r < 0) {
      printf("Failed to parse JSON: %d\r\n", r);
      return NULL;
    }

    /* Assume the top-level element is an object */
    /*if (r < 1 || token[0].type != JSMN_OBJECT) {
      printf("r=%d, type=%d\r\r\n", r, token[0].type);
      printf("Object expected\r\n");
      return NULL;
    }*/

    /* Loop over all keys of the root object */
    for (i = 0; i < r; i++) {
    	if (jsoneq(json_msg, &token[i], "ok") == 0) {
			my_strcpy(json_msg + token[i + 1].start, temp_str, 10);
			if ( (ret = return_true_or_false_from_str(temp_str)) == -1)
			{
				printf("string is not true or false\r\n");
			}
			else
			{
				new_msg -> ok = ret;
			}
			i++;
    	} else if (jsoneq(json_msg, &token[i], "name") == 0) {
            my_strcpy(json_msg + token[i + 1].start, new_msg->customer.name, JSON_ITEM_MAX_SIZE);
            i++;
    	} else if (jsoneq(json_msg, &token[i], "email") == 0) {
            my_strcpy(json_msg + token[i + 1].start, new_msg->customer.email, JSON_ITEM_MAX_SIZE);
            i++;
    	} else if (jsoneq(json_msg, &token[i], "id") == 0) {
            my_strcpy(json_msg + token[i + 1].start, new_msg->customer.id, 8);
            i++;
    	} else if (jsoneq(json_msg, &token[i], "numPeople") == 0) {
    		new_msg -> customer.numPeople = atoi(json_msg + token[i+1].start);
    	        i++;
    	} else if (jsoneq(json_msg, &token[i], "isCheckingIn") == 0) {
            my_strcpy(json_msg + token[i + 1].start, temp_str, 10);
            if ( (ret = return_true_or_false_from_str(temp_str)) == -1)
            {
                printf("string is not true or false\r\n");
            }
            else
            {
            	new_msg -> isCheckingIn = ret;
            }
            i++;
    	}
    }

    return new_msg;
}

// convert no data type response
no_data_server_msg* no_data_parse_json(char *json_msg)
{
    int i;
    int r;
    int ret;
    jsmn_parser p;
    jsmntok_t token[1024]; /* We expect no more than 1024 tokens */
    char temp_str[8];

    no_data_server_msg* new_msg = malloc(sizeof (no_data_server_msg));
    if (new_msg == NULL)
        return NULL;

    jsmn_init(&p);
    r = jsmn_parse(&p, json_msg, strlen(json_msg), token,
                  sizeof(token) / sizeof(token[0]));
    if (r < 0) {
      printf("Failed to parse JSON: %d\r\n", r);
      return NULL;
    }

    /* Assume the top-level element is an object */
    /*if (r < 1 || token[0].type != JSMN_OBJECT) {
	  printf("r=%d, type=%d\r\r\n", r, token[0].type);
	  printf("Object expected\r\n");
	  return NULL;
	}*/

	/* Loop over all keys of the root object */
	for (i = 0; i < r; i++) {
    	if (jsoneq(json_msg, &token[i], "ok") == 0) {
			my_strcpy(json_msg + token[i + 1].start, temp_str, 10);
			if ( (ret = return_true_or_false_from_str(temp_str)) == -1)
			{
				printf("string is not true or false\r\n");
				printf("%s\r\n", temp_str);
			}
			else
			{
				new_msg -> ok = ret;
			}
			i++;
    	}
    }

    return new_msg;
}

// convert queue status response
status_server_msg* status_parse_json(char *json_msg)
{
    int i;
    int r;
    int ret;
    jsmn_parser p;
    jsmntok_t token[1024]; /* We expect no more than 1024 tokens */
    char temp_str[8];

    status_server_msg* new_msg = malloc(sizeof (status_server_msg));
    if (new_msg == NULL)
        return NULL;

    jsmn_init(&p);
    r = jsmn_parse(&p, json_msg, strlen(json_msg), token,
                  sizeof(token) / sizeof(token[0]));
    if (r < 0) {
      printf("Failed to parse JSON: %d\r\n", r);
      return NULL;
    }

    /* Assume the top-level element is an object */
    /*if (r < 1 || token[0].type != JSMN_OBJECT) {
	  printf("r=%d, type=%d\r\r\n", r, token[0].type);
	  printf("Object expected\r\n");
	  return NULL;
	}*/

	/* Loop over all keys of the root object */
	for (i = 0; i < r; i++) {
    	if (jsoneq(json_msg, &token[i], "ok") == 0) {
			my_strcpy(json_msg + token[i + 1].start, temp_str, 10);
			if ( (ret = return_true_or_false_from_str(temp_str)) == -1)
			{
				printf("string is not true or false\r\n");
			}
			else
			{
				new_msg -> ok = ret;
			}
			i++;
    	} else if (jsoneq(json_msg, &token[i], "queueLength") == 0) {
    		new_msg -> queueLength = atoi(json_msg + token[i+1].start);
    	        i++;
    	} else if (jsoneq(json_msg, &token[i], "numPeopleInStore") == 0) {
    		new_msg -> numPeopleInStore = atoi(json_msg + token[i+1].start);
    	        i++;
    	} else if (jsoneq(json_msg, &token[i], "maxCapacity") == 0) {
    		new_msg -> maxCapacity = atoi(json_msg + token[i+1].start);
    	        i++;
    	}
    }

    return new_msg;
}

void print_out_barcode_msg(barcode_server_msg* msg)
{
    printf("ok: %d\r\n", msg->ok);
    printf("customer name: %s\r\n", msg->customer.name);
    printf("customer email: %s\r\n", msg->customer.email);
    printf("customer id: %s\r\n", msg->customer.id);
    printf("customer numPeople: %d\r\n", msg->customer.numPeople);
    printf("isCheckingIn: %d\r\n", msg->isCheckingIn);
}

void print_out_no_data_msg(no_data_server_msg* msg)
{
    printf("ok: %d\r\n", msg->ok);
}

void print_out_status_msg(status_server_msg* msg)
{
	printf("ok: %d\r\n", msg->ok);
	printf("queueLength: %d\r\n", msg->queueLength);
	printf("numPeopleInStore: %d\r\n", msg->numPeopleInStore);
	printf("maxCapacity: %d\r\n", msg->maxCapacity);
}





