
#pragma once
#define MAX_STATUS_CODE_LEN 10
#define MAX_PHRASE_CODE_LEN 50
#define MAX_PAYLOAD_LEN 8192
#include <stdlib.h>

struct response
{
    char status_code[MAX_STATUS_CODE_LEN];
    char phrase[MAX_PHRASE_CODE_LEN];
    char payload[MAX_PAYLOAD_LEN];
};

typedef struct response response_t;


