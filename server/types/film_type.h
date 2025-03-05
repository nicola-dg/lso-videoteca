
#pragma once
#include <stdlib.h>

struct film
{
    char id[100];
    char title[100];
    char genre[100];
    char total_copies[100];
    char available_copies[100];
    char loan_count[100];
};

typedef struct film film_t;