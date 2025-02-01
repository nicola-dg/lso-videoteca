#pragma once
#include "../types/request_type.h"
#include "../request_handler/request_handler.h"

void route_post_request(request_t *req, int client_socket);
void route_request(request_t *req, int client_socket);