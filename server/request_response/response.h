#pragma once
#include "../types/response_type.h"
#include "../json_parsing/json.h"
#include "../socket/socket.h"
#include <stdbool.h>

response_t *init_response();
void free_response(response_t *res);
char *serialize_response(response_t *res);
bool send_response(response_t *res, int client_socket);