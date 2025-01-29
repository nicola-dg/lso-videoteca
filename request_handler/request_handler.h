#pragma once
#include "../types/request_type.h"
#include <stdbool.h>
#include "../json_parsing/request_print.h"


bool handle_post_user_request(request_t * req);
bool handle_post_film_request(request_t * req);
bool handle_post_message_request(request_t * req);