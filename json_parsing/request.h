#pragma once
#include "json.h"

// Funzioni per la gestione della richiesta
request_t *init_request();
void free_request(request_t *req);