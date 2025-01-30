#pragma once
#include "../types/response_type.h"
#include <stdbool.h>

response_t *init_response();
void free_response(response_t *res);