#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json.h"

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
    int ret;
    
    from_server_msg test_server_struct;
    ret = fill_in_struct_from_json(STM32_JSON, &test_server_struct);
    if(ret != 0)
    {
      return EXIT_FAILURE;
    }
    print_out_from_server_msg(&test_server_struct);
    
    return EXIT_SUCCESS;
}

