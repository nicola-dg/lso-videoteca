
#pragma once
#include <stdlib.h>

struct user
{
    char name[100];
    char surname[100];
    char username[100];
    char email[100];
    char password[100];
};

typedef struct user user_t;