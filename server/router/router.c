#include "router.h"

void route_get_request(request_t *req, int client_socket)
{
    if (strcmp(req->path, "/user") == 0)
    {
        handle_get_user_request(req, client_socket);
    }
    else if (strcmp(req->path, "/film") == 0)
    {
        handle_get_film_request(req, client_socket);
    }
    else if (strcmp(req->path, "/loan/expire") == 0)
    {
        handle_get_loan_expire_request(req, client_socket);
    }
    else if (strcmp(req->path, "/message") == 0)
    {
        handle_get_message_request(req, client_socket);
    }
    else if (strcmp(req->path, "/loan") == 0)
    {
        handle_get_loan_request(req, client_socket);
    }
}

void route_post_request(request_t *req, int client_socket)
{
    if (strcmp(req->path, "/user") == 0)
    {
        handle_post_user_request(req, client_socket);
    }
    else if (strcmp(req->path, "/film") == 0)
    {
        handle_post_film_request(req, client_socket);
    }
    else if (strcmp(req->path, "/message") == 0)
    {
        handle_post_message_request(req, client_socket);
    }
    else if (strcmp(req->path, "/cart/film") == 0)
    {
        handle_post_cart_film_request(req, client_socket);
    }
    else if (strcmp(req->path, "/loan/film") == 0)
    {
        handle_post_loan_film_request(req, client_socket);
    }
}

void route_put_request(request_t *req, int client_socket)
{
    if (strcmp(req->path, "/user") == 0)
    {
        handle_put_user_request(req, client_socket);
    }
    else if (strcmp(req->path, "/loan/film") == 0)
    {
        handle_put_loan_film_request(req, client_socket);
    }
    else if (strcmp(req->path, "/loan") == 0)
    {
        handle_put_loan_request(req, client_socket);
    }
}

void route_delete_request(request_t *req, int client_socket)
{
    if (strcmp(req->path, "/user") == 0)
    {
        handle_delete_user_request(req, client_socket);
    }
}

void route_request(request_t *req, int client_socket)
{
    if (req)
    {
        if (strcmp(req->method, "GET") == 0)
        {
            route_get_request(req, client_socket);
        }
        else if (strcmp(req->method, "POST") == 0)
        {
            route_post_request(req, client_socket);
        }
        else if (strcmp(req->method, "PUT") == 0)
        {
            route_put_request(req, client_socket);
        }
        else if (strcmp(req->method, "DELETE") == 0)
        {
            route_delete_request(req, client_socket);
        }
    }
}
