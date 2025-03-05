#pragma once
#include "../json_parsing/json.h"
#include "../types/user_type.h"

// Funzioni per la gestione della richiesta
request_t *init_request();
void free_request(request_t *req);
bool is_valid_user(user_t *user);