
#include "response.h"

response_t *init_response()
{
    response_t *response = (response_t *)malloc(sizeof(response_t));
    return response;
}

void free_response(response_t *res)
{
    if (res)
    {
        free(res);
    }
}

bool send_response(response_t* res){
    
}