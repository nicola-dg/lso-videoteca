#pragma once
#include "../types/request_type.h"
#include <stdbool.h>
#include "../json_parsing/request_print.h"

/*---------------------- POST --------------------------*/
bool handle_post_user_request(request_t *req);
bool handle_post_film_request(request_t *req);
bool handle_post_message_request(request_t *req);
/*------------------------------------------------------*/

/*---------------------- GET --------------------------*/
bool handle_get_user_request(request_t *req);
bool handle_get_film_request(request_t *req);
bool handle_get_cart_film_request(request_t *req);
bool handle_get_loan_film_request(request_t *req);
bool handle_get_film_info_request(request_t *req);
bool handle_get_loan_expire_request(request_t *req);
bool handle_get_message_request(request_t *req);
/*------------------------------------------------------*/

/*---------------------- PUT --------------------------*/
bool handle_put_user_request(request_t *req);
bool handle_put_message_request(request_t *req);
bool handle_put_loan_request(request_t *req);
/*------------------------------------------------------*/

/*---------------------- DELETE --------------------------*/
bool handle_delete_user_request(request_t *req);