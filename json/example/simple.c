#include "../jsmn.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * A small example of jsmn parsing when JSON structure is known and number of
 * tokens is predictable.
 */

static const char *JSON_STRING =
   "{\n"
"   \"user\":\"ok\",\n"
"   \"admin\":false\n"
"}";

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


static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
  if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
      strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
    return 0;
  }
  return -1;
}

int printSTM32_JSON()
{
   int i;
    int r;
    jsmn_parser p;
    jsmntok_t token[1028]; /* We expect no more than 128 tokens */

    jsmn_init(&p);
    r = jsmn_parse(&p, STM32_JSON, strlen(STM32_JSON), token,
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
      if (jsoneq(STM32_JSON, &token[i], "status") == 0) {
        /* We may use strndup() to fetch string value */
        printf("- status: %.*s\n", token[i + 1].end - token[i + 1].start,
              STM32_JSON + token[i + 1].start);
        i++;
      } 
      else if (jsoneq(STM32_JSON, &token[i], "isToMicroController") == 0) {
        /* We may additionally check if the value is either "true" or "false" */
        printf("- isToMicroController: %.*s\n", token[i + 1].end - token[i + 1].start,
              STM32_JSON + token[i + 1].start);
        i++;
      } 
      else if (jsoneq(STM32_JSON, &token[i], "customer") == 0) {
        printf("- customer: {\n");
        i++;
      } 
       else if (jsoneq(STM32_JSON, &token[i], "id") == 0) {
        printf("\t id: %.*s\n", token[i + 1].end - token[i + 1].start,
              STM32_JSON + token[i + 1].start);
        i++;
      } 
      else if (jsoneq(STM32_JSON, &token[i], "numOfCustomer") == 0) {
        printf("\t numOfCustomer: %.*s\n", token[i + 1].end - token[i + 1].start,
              STM32_JSON + token[i + 1].start);
        i++;
      }
      else if (jsoneq(STM32_JSON, &token[i], "isTimeToCheckIn") == 0) {
        printf("\t isTimeToCheckIn: %.*s\n", token[i + 1].end - token[i + 1].start,
              STM32_JSON + token[i + 1].start);
        i++;
      } 
      else if (jsoneq(STM32_JSON, &token[i], "isNewCustomer") == 0) {
        printf("\t isNewCustomer: %.*s\n", token[i + 1].end - token[i + 1].start,
              STM32_JSON + token[i + 1].start);
        i++;
      }              
      else if (jsoneq(STM32_JSON, &token[i], "isOnQueue") == 0) {
        printf("\t isOnQueue: %.*s\n", token[i + 1].end - token[i + 1].start,
              STM32_JSON + token[i + 1].start);
        i++;
      }  
      else if (jsoneq(STM32_JSON, &token[i], "queueNum") == 0) {
        printf("\t queueNum: %.*s\n      \t}\n", token[i + 1].end - token[i + 1].start,
              STM32_JSON + token[i + 1].start);
        i++;
      }  
      else if (jsoneq(STM32_JSON, &token[i], "errMsg") == 0) {
        /* We may want to do strtol() here to get numeric value */
        printf("- errMsg: %.*s\n", token[i + 1].end - token[i + 1].start,
              STM32_JSON + token[i + 1].start);
        i++;
      } 
    }

  
}

int printJSON_STRING()
{
    int i;
    int r;
    jsmn_parser p;
    jsmntok_t token[128]; /* We expect no more than 128 tokens */

    jsmn_init(&p);
    r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), token,
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
      if (jsoneq(JSON_STRING, &token[i], "user") == 0) {
        /* We may use strndup() to fetch string value */
        printf("- User: %.*s\n", token[i + 1].end - token[i + 1].start,
              JSON_STRING + token[i + 1].start);
        i++;
      } else if (jsoneq(JSON_STRING, &token[i], "admin") == 0) {
        /* We may additionally check if the value is either "true" or "false" */
        printf("- Admin: %.*s\n", token[i + 1].end - token[i + 1].start,
              JSON_STRING + token[i + 1].start);
        i++;
      } else if (jsoneq(JSON_STRING, &token[i], "uid") == 0) {
        /* We may want to do strtol() here to get numeric value */
        printf("- UID: %.*s\n", token[i + 1].end - token[i + 1].start,
              JSON_STRING + token[i + 1].start);
        i++;
      } else if (jsoneq(JSON_STRING, &token[i], "groups") == 0) {
        int j;
        printf("- Groups:\n");
        if (token[i + 1].type != JSMN_ARRAY) {
          continue; /* We expect groups to be an array of strings */
        }
        for (j = 0; j < token[i + 1].size; j++) {
          jsmntok_t *g = &token[i + j + 2];
          printf("  * %.*s\n", g->end - g->start, JSON_STRING + g->start);
        }
        i += token[i + 1].size + 1;
      } else {
        printf("Unexpected key: %.*s\n", token[i].end - token[i].start,
              JSON_STRING + token[i].start);
      }
    }
}

int main() {
  printSTM32_JSON();
  return EXIT_SUCCESS;
}
