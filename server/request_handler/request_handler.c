#include "request_handler.h"

/*------------------------------------------------------------------*/
/*-------------------------- POST REQUESTS -------------------------*/
/*------------------------------------------------------------------*/

bool handle_post_user_request(request_t *req, int client_socket)
{
    response_t *res = init_response();

    user_t *user = extract_user_from_json(req->payload);

    if (is_valid_user(user))
    {
        insert_user(user->username, user->password, user->email, user->name, user->surname, user->role);
        strcpy(res->status_code, "200");
        strcpy(res->phrase, "Ok");
        strcpy(res->payload, req->payload);
    }
    else
    {
        strcpy(res->status_code, "400");
        strcpy(res->phrase, "Bad Request");
        strcpy(res->payload, "Invalid Credentials");
    }

    send_response(res, client_socket);
    free_request(req);
    free_response(res);

    return true;
}

bool handle_post_film_request(request_t *req, int client_socket)
{
    printf("Controllo se l'utente è un NEGOZIANTE...\n");
    printf("Controllo se il film è formattato correttamente...\n");
    printf("Aggiungo nuovo film...\n");
    // Aggiungi qui il codice per gestire la richiesta POST
    print_request(req);

    // liberare la memoria della request (free_request(req))
    return true;
}

bool handle_post_message_request(request_t *req, int client_socket)
{
    printf("Controllo se l'utente è un NEGOZIANTE...\n");
    printf("Controllo se il messaggio è formattato correttamente...\n");
    printf("Aggiungo nuovo messaggio...\n");
    // Aggiungi qui il codice per gestire la richiesta POST
    print_request(req);

    // liberare la memoria della request (free_request(req))
    return true;
}

/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/*-------------------------- GET REQUESTS --------------------------*/
/*------------------------------------------------------------------*/

bool handle_get_user_request(request_t *req, int client_socket)
{
    response_t *res = init_response();
    user_t *user = extract_user_from_json(req->payload);

    if (user->username && user->password && user->id && user->role)
    {
        jwt_payload_t *payload = init_jwt_payload(user->id, user->role);
        jwt_t *jwt = init_jwt(payload);
        if (select_user_by_username_and_password(user->username, user->password))
        {
            strcpy(res->status_code, "200");
            strcpy(res->phrase, "Ok");
            strcpy(res->payload, jwt_encode_str(jwt));
        }

        jwt_free(jwt);
    }
    else
    {
        strcpy(res->status_code, "400");
        strcpy(res->phrase, "Bad Request");
        strcpy(res->payload, "Invalid Credentials");
    }

    send_response(res, client_socket);
    free_request(req);
    free_response(res);

    return true;
}

bool handle_get_film_request(request_t *req, int client_socket)
{
    printf("GET /film request ricevuta...\n");
    // Aggiungi qui il codice per gestire la richiesta GET
    print_request(req);

    response_t *res = init_response();
    char* films = select_all_films();
    if(films){
        strcpy(res->status_code, "200");
        strcpy(res->phrase, "Ok");
        strcpy(res->payload, films);  
    }else
    {
        strcpy(res->status_code, "500");
        strcpy(res->phrase, "Server Error");
    }


    send_response(res, client_socket);
    free_request(req);
    free_response(res);

    return true;
}

bool handle_get_cart_film_request(request_t *req, int client_socket)
{
    printf("Controllo se l'utente è un USER...\n");
    printf("GET /cart/film request ricevuta...\n");
    // Aggiungi qui il codice per gestire la richiesta GET
    print_request(req);

    // liberare la memoria della request (free_request(req))
    return true;
}

bool handle_get_loan_film_request(request_t *req, int client_socket)
{
    printf("Controllo se l'utente è un USER...\n");
    printf("GET /loan/film request ricevuta...\n");
    // Aggiungi qui il codice per gestire la richiesta GET
    print_request(req);

    // liberare la memoria della request (free_request(req))
    return true;
}

bool handle_get_film_info_request(request_t *req, int client_socket)
{
    printf("Controllo se l'utente è un NEGOZIANTE...\n");
    printf("GET /film/info request ricevuta...\n");
    // Aggiungi qui il codice per gestire la richiesta GET
    print_request(req);

    // liberare la memoria della request (free_request(req))
    return true;
}

bool handle_get_loan_expire_request(request_t *req, int client_socket)
{
    printf("Controllo se l'utente è un NEGOZIANTE...\n");
    printf("GET /loan/expired request ricevuta...\n");
    // Aggiungi qui il codice per gestire la richiesta GET
    print_request(req);

    // liberare la memoria della request (free_request(req))
    return true;
}

bool handle_get_message_request(request_t *req, int client_socket)
{
    printf("Controllo se l'utente è un USER...\n");
    printf("GET /message request ricevuta...\n");
    // Aggiungi qui il codice per gestire la richiesta GET
    print_request(req);

    // liberare la memoria della request (free_request(req))
    return true;
}

/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/*-------------------------- PUT REQUESTS --------------------------*/
/*------------------------------------------------------------------*/

bool handle_put_user_request(request_t *req, int client_socket)
{
    printf("Controllo se l'utente è un NEGOZIANTE...\n");
    printf("PUT /user request ricevuta...\n");
    // Aggiungi qui il codice per gestire la richiesta PUT
    print_request(req);

    // liberare la memoria della request (free_request(req))
    return true;
}

bool handle_put_loan_film_request(request_t *req, int client_socket)
{
    printf("PUT /loan/film request ricevuta...\n");
    // Aggiungi qui il codice per gestire la richiesta PUT
    print_request(req);

    // liberare la memoria della request (free_request(req))
    return true;
}

bool handle_put_loan_request(request_t *req, int client_socket)
{
    printf("PUT /loan request ricevuta...\n");
    // Aggiungi qui il codice per gestire la richiesta PUT
    print_request(req);

    // liberare la memoria della request (free_request(req))
    return true;
}

/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/*------------------------ DELETE REQUESTS -------------------------*/
/*------------------------------------------------------------------*/

bool handle_delete_user_request(request_t *req, int client_socket)
{
    printf("DELETE /user request ricevuta...\n");
    // Aggiungi qui il codice per gestire la richiesta DELETE
    print_request(req);

    // liberare la memoria della request (free_request(req))
    return true;
}
