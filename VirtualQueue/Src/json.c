#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jsmn.h"
#include "json.h"


// TODO: replace printf to LCD display

static const char *STM32_JSON = 
  "{\n"
"   \"status\":\"ok\",\n"
"   \"isToMicroController\":true,\n"
"   \"customer\":{\n"
"      \"id\":40,\n"
"      \"numOfCustomer\":4,\n"
"      \"isTimeToCheckIn\":false,\n"
"      \"isNewCustomer\":true,\n"
"      \"isOnQueue\":true,\n"
"      \"queueNum\":4\n"
"   },\n"
"   \"errMsg\":\"Testing\"\n"
"}";

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
    if(strncmp(str, "true",5) == 0)
        return true;
    if(strncmp(str, "false",6) == 0)
        return false;
    printf("%s\n",str);
    return -1;
}

/**
 * fill_in_struct_from_json take in JSON string and fills all the information
 * from the JSON message to a struct
 * 
 * return 1 if fail
 * return 0 if success
 */
int fill_in_struct_from_json(char *json_msg, from_server_msg* new_server_msg)
{
    int i;
    int r;
    int ret;
    jsmn_parser p;
    jsmntok_t token[1024]; /* We expect no more than 1024 tokens */
    char temp_str[8];

    jsmn_init(&p);
    r = jsmn_parse(&p, json_msg, strlen(json_msg), token,
                  sizeof(token) / sizeof(token[0]));
    if (r < 0) {
      printf("Failed to parse JSON: %d\n", r);
      return 1;
    }

    /* Assume the top-level element is an object */
    if (r < 1 || token[0].type != JSMN_OBJECT) {
      printf("Object expected\n");
      return 1;
    }

    /* Loop over all keys of the root object */
    for (i = 1; i < r; i++) {
      if (jsoneq(json_msg, &token[i], "status") == 0) {
        my_strcpy(json_msg + token[i + 1].start,new_server_msg-> status, 8);
        i++;
      } 
      else if (jsoneq(json_msg, &token[i], "isToMicroController") == 0) {
        my_strcpy(json_msg + token[i + 1].start, temp_str, 10);
        if ( (ret = return_true_or_false_from_str(temp_str)) == -1)
        {
            printf("string is not true or false\n");
        }
        else
        {
            new_server_msg -> isToMicroController = ret;
        }
        i++;
      } 
       else if (jsoneq(json_msg, &token[i], "id") == 0) {
        new_server_msg -> customer.id = atoi(json_msg + token[i+1].start);
        i++;
      } 
      else if (jsoneq(json_msg, &token[i], "numOfCustomer") == 0) {
        new_server_msg -> customer.numOfCustomer = atoi(json_msg + token[i+1].start);
        i++;
      }
      else if (jsoneq(json_msg, &token[i], "isTimeToCheckIn") == 0) {
        my_strcpy(json_msg + token[i + 1].start, temp_str, 10);
        if ( (ret = return_true_or_false_from_str(temp_str)) == -1)
        {
            printf("string is not true or false\n");
        }
        else
        {
            new_server_msg -> customer.isTimeToCheckIn = ret;
        }
        i++;
      } 
      else if (jsoneq(json_msg, &token[i], "isNewCustomer") == 0) {
        my_strcpy(json_msg + token[i + 1].start, temp_str, 10);
        if ( (ret = return_true_or_false_from_str(temp_str)) == -1)
        {
            printf("string is not true or false\n");
        }
        else
        {
            new_server_msg -> customer.isNewCustomer = ret;
        }
        i++;
      }              
      else if (jsoneq(json_msg, &token[i], "isOnQueue") == 0) {
        my_strcpy(json_msg + token[i + 1].start, temp_str, 10);
        if ( (ret = return_true_or_false_from_str(temp_str)) == -1)
        {
            printf("string is not true or false\n");
        }
        else
        {
            new_server_msg -> customer.isOnQueue = ret;
        }
        i++;
      }  
      else if (jsoneq(json_msg, &token[i], "queueNum") == 0) {
        new_server_msg -> customer.queueNum = atoi(json_msg + token[i+1].start);
        i++;
      }  
      else if (jsoneq(json_msg, &token[i], "errMsg") == 0) {
        my_strcpy(json_msg + token[i + 1].start, new_server_msg-> errMsg, 128);
        i++;
      } 
    }

    return 0;
}

void print_out_from_server_msg(from_server_msg *server_msg_struct)
{
    printf("status: %s\n", server_msg_struct->status);
    printf("isToMicroController %d\n", server_msg_struct->isToMicroController);
    printf("customer id: %d\n", server_msg_struct->customer.id);
    printf("customer numOfCustomer: %d\n", server_msg_struct->customer.numOfCustomer);
    printf("customer isTimeToCheckIn: %d\n", server_msg_struct->customer.isTimeToCheckIn);
    printf("customer isNewCustomer: %d\n", server_msg_struct->customer.isNewCustomer);
    printf("customer isOnQueue: %d\n", server_msg_struct->customer.isOnQueue);
    printf("customer queueNum: %d\n", server_msg_struct->customer.queueNum);
    printf("errMsg: %s\n", server_msg_struct->errMsg);
}





