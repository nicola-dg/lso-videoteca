#pragma once
#include "../types/request_type.h"
#include "../request_response/request_print.h"
#include "../request_response/response.h"
#include "../jwt/jwt.h"
#include <stdbool.h>

/*---------------------- POST --------------------------*/
bool handle_post_user_request(request_t *req, int client_socket);
bool handle_post_film_request(request_t *req, int client_socket);
bool handle_post_message_request(request_t *req, int client_socket);
/*------------------------------------------------------*/

/*---------------------- GET --------------------------*/
bool handle_get_user_request(request_t *req, int client_socket);
bool handle_get_film_request(request_t *req, int client_socket);
bool handle_get_cart_film_request(request_t *req, int client_socket);
bool handle_get_loan_film_request(request_t *req, int client_socket);
bool handle_get_film_info_request(request_t *req, int client_socket);
bool handle_get_loan_expire_request(request_t *req, int client_socket);
bool handle_get_message_request(request_t *req, int client_socket);
/*------------------------------------------------------*/

/*---------------------- PUT --------------------------*/
bool handle_put_user_request(request_t *req, int client_socket);
bool handle_put_loan_film_request(request_t *req, int client_socket);
bool handle_put_loan_request(request_t *req, int client_socket);
/*------------------------------------------------------*/

/*---------------------- DELETE --------------------------*/
bool handle_delete_user_request(request_t *req, int client_socket);