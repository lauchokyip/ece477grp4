#include "json.h"
#include "jsmn.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

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


int main()
{
    from_server_msg* test_server_struct = fill_in_struct_from_json(STM32_JSON);
    if (test_server_struct == NULL) 
        return EXIT_FAILURE; 
    print_out_from_server_msg(test_server_struct);
    free(test_server_struct);
    return EXIT_SUCCESS;
}

