
#pragma once
#include <stdlib.h>

struct message
{
    char id[100];
    char user_id[100];
    char text[100];
    char checkout_date[100];
};

typedef struct message message_t;