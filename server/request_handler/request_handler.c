#include "request_handler.h"

/*------------------------------------------------------------------*/
/*-------------------------- POST REQUESTS -------------------------*/
/*------------------------------------------------------------------*/

bool handle_post_user_request(request_t *req, int client_socket)
{
    response_t *res = init_response();
    user_t *user = extract_user_from_json(req->payload);
    bool ret = false;

    if (is_valid_user(user) && insert_user(user->username, user->password, user->email, user->name, user->surname))
    {
        strcpy(res->status_code, "200");
        strcpy(res->phrase, "Ok");
        strcpy(res->payload, req->payload);
        ret = true;
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
    free(user);

    return ret;
}

bool handle_post_film_request(request_t *req, int client_socket)
{
    printf("POST /film RICEVUTA\n ");
    response_t *res = init_response();
    bool ret = false;
    print_request(req);

    if (isUser(req)) // TODO: DEVE CONTROLLARE CHE SIA NEGOZIANTE METTO USER PER TEST
    {
        printf("here");
        film_t *film = extract_film_from_json(req->payload);
        if (insert_film(film->title, film->genre, film->price))
        {
            strcpy(res->status_code, "200");
            strcpy(res->phrase, "ok");
            strcpy(res->payload, req->payload);
            ret = true;
        }
        else
        {
            strcpy(res->status_code, "500");
            strcpy(res->phrase, "Server Error");
        }

        free(film);
    }
    else
    {
        strcpy(res->status_code, "403");
        strcpy(res->phrase, "Forbidden");
        strcpy(res->payload, "Non hai i permessi necessari per accedere a questa risorsa.");
    }

    send_response(res, client_socket);
    free_request(req);
    free_response(res);

    return ret;
}

bool handle_post_message_request(request_t *req, int client_socket)
{

    response_t *res = init_response();
    bool ret = false;

    if (isUser(req))
    {
        message_t *message = extract_message_from_json(req->payload);
        if (insert_message("1", message->text)) // TODO: USER ID VA ESTRATTO DA MESSAGE -> ID
        {
            strcpy(res->status_code, "200");
            strcpy(res->phrase, "ok");
            ret = true;
        }
        else
        {
            strcpy(res->status_code, "400");
            strcpy(res->phrase, "Can't add to cart");
        }
        free(message);
    }
    else
    {
        strcpy(res->status_code, "402");
        strcpy(res->phrase, "Don't have permissions");
    }

    send_response(res, client_socket);
    free_request(req);
    free_response(res);
    return ret;
}

bool handle_post_cart_film_request(request_t *req, int client_socket)
{

    response_t *res = init_response();
    bool ret = false;
    if (isUser(req))
    {
        film_t *film = extract_film_from_json(req->payload);
        char *film_id = malloc(sizeof(char) * 100);
        select_film_id_by_title(film->title, film_id);
        if (!film_id)
        {
            return false;
        }
        if (insert_cart(film_id, jwt_extract_user_id(decode_jwt(extract_jwt_from_headers(req)))))
        {
            strcpy(res->status_code, "200");
            strcpy(res->phrase, "ok");
            ret = true;
        }
        else
        {
            strcpy(res->status_code, "400");
            strcpy(res->phrase, "Can't add to cart");
        }
        free(film);
        free(film_id);
    }
    else
    {
        strcpy(res->status_code, "402");
        strcpy(res->phrase, "Don't have permissions");
    }

    send_response(res, client_socket);
    free_request(req);
    free_response(res);
    return ret;
}

bool handle_post_loan_film_request(request_t *req, int client_socket)
{

    response_t *res = init_response();
    bool ret = false;

    if (isUser(req))
    {
        film_t *film = extract_film_from_json(req->payload);

        if (insert_loan(film->id, jwt_extract_user_id(decode_jwt(extract_jwt_from_headers(req)))))
        {
            strcpy(res->status_code, "200");
            strcpy(res->phrase, "ok");
            ret = true;
        }
        else
        {
            strcpy(res->status_code, "400");
            strcpy(res->phrase, "Can't add to cart");
        }
        free(film);
    }
    else
    {
        strcpy(res->status_code, "402");
        strcpy(res->phrase, "Don't have permissions");
    }

    send_response(res, client_socket);
    free_request(req);
    free_response(res);
    return ret;
}
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/*-------------------------- GET REQUESTS --------------------------*/
/*------------------------------------------------------------------*/

bool handle_get_user_request(request_t *req, int client_socket)
{
    response_t *res = init_response();
    user_t *user = extract_user_from_json(req->payload);
    select_user_info_by_username(user->username, user->id, user->role);
    bool ret;

    if (user->username && user->password && user->id && user->role)
    {
        jwt_payload_t *payload = init_jwt_payload(user->id, user->role);
        jwt_t *jwt = init_jwt(payload);
        if (select_user_by_username_and_password(user->username, user->password))
        {
            strcpy(res->status_code, "200");
            strcpy(res->phrase, "Ok");
            strcpy(res->payload, jwt_encode_str(jwt));
            ret = true;
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

    return ret;
}

bool handle_get_film_request(request_t *req, int client_socket)
{

    response_t *res = init_response();
    bool ret = false;

    if (isUser(req))
    {
        char *films = select_all_films();

        if (films == NULL)
        {
            // Errore del server nel recupero dei film
            strcpy(res->status_code, "500");
            strcpy(res->phrase, "Server Error");
            strcpy(res->payload, "Errore del server, riprova piu tardi.");
        }
        else if (strlen(films) == 0)
        {
            // Nessun film trovato
            strcpy(res->status_code, "404");
            strcpy(res->phrase, "Not Found");
            strcpy(res->payload, "Nessun film disponibile.");
        }
        else
        {
            // Film trovati con successo
            strcpy(res->status_code, "200");
            strcpy(res->phrase, "Ok");
            strcpy(res->payload, films);
            ret = true;
        }
        free(films); // Libera la memoria allocata per la stringa dei film
    }
    else
    {
        strcpy(res->status_code, "403");
        strcpy(res->phrase, "Forbidden");
        strcpy(res->payload, "Non hai i permessi necessari per accedere a questa risorsa.");
    }

    send_response(res, client_socket);

    free_request(req);
    free_response(res);

    return ret;
}

bool handle_get_loan_expire_request(request_t *req, int client_socket)
{
    response_t *res = init_response();
    bool ret = false;

    if (isNegoziante(req))
    {
        char *expired_loans = select_all_expired_loans();

        if (expired_loans == NULL)
        {
            strcpy(res->status_code, "500");
            strcpy(res->phrase, "Server Error");
            strcpy(res->payload, "Errore del server, riprova piu tardi.");
        }
        else if (strlen(expired_loans) == 0)
        {
            strcpy(res->status_code, "404");
            strcpy(res->phrase, "Not Found");
            strcpy(res->payload, "Nessun noleggio scaduto.");
        }
        else
        {
            strcpy(res->status_code, "200");
            strcpy(res->phrase, "Ok");
            strcpy(res->payload, expired_loans);
            ret = true;
        }

        free(expired_loans);
    }
    else
    {
        strcpy(res->status_code, "403");
        strcpy(res->phrase, "Forbidden");
        strcpy(res->payload, "Non hai i permessi necessari per accedere a questa risorsa.");
    }

    send_response(res, client_socket);

    free_request(req);
    free_response(res);

    return ret;
}

// TODO: implementa
bool handle_get_message_request(request_t *req, int client_socket)
{
    printf("Controllo se l'utente è un USER...\n");
    printf("GET /message request ricevuta...\n");
    // Aggiungi qui il codice per gestire la richiesta GET
    print_request(req);

    // liberare la memoria della request (free_request(req))
    return true;
}

bool handle_get_loan_request(request_t *req, int client_socket)
{

    response_t *res = init_response();
    bool ret = false;

    if (isUser(req))
    {
        char *loans = select_active_loans_by_id(jwt_extract_user_id(decode_jwt(extract_jwt_from_headers(req))));

        if (loans == NULL)
        {
            strcpy(res->status_code, "500");
            strcpy(res->phrase, "Server Error");
            strcpy(res->payload, "Errore del server, riprova piu tardi.");
        }
        else if (strlen(loans) == 0)
        {
            strcpy(res->status_code, "404");
            strcpy(res->phrase, "Not Found");
            strcpy(res->payload, "Nessun film disponibile.");
        }
        else
        {
            strcpy(res->status_code, "200");
            strcpy(res->phrase, "Ok");
            strcpy(res->payload, loans);
            ret = true;
        }

        free(loans);
    }
    else
    {
        strcpy(res->status_code, "403");
        strcpy(res->phrase, "Forbidden");
        strcpy(res->payload, "Non hai i permessi necessari per accedere a questa risorsa.");
    }

    send_response(res, client_socket);

    free_request(req);
    free_response(res);

    return ret;
}

/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/*-------------------------- PUT REQUESTS --------------------------*/
/*------------------------------------------------------------------*/

// TODO: implementa
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

    response_t *res = init_response();
    bool ret = false;

    if (isUser(req))
    {
        film_t *film = extract_film_from_json(req->payload);
        if (update_film_return(film->id, jwt_extract_user_id(decode_jwt(extract_jwt_from_headers(req)))))
        {
            strcpy(res->status_code, "200");
            strcpy(res->phrase, "ok");
            ret = true;
        }
        else
        {
            strcpy(res->status_code, "400");
            strcpy(res->phrase, "Can't add to cart");
        }
        free(film);
    }
    else
    {
        strcpy(res->status_code, "403");
        strcpy(res->phrase, "Forbidden");
        strcpy(res->payload, "You dont't have perimssion to access this resource.");
    }

    send_response(res, client_socket);
    free_request(req);
    free_response(res);
    return ret;
}

// TODO: implementa
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

// TODO: implementa
bool handle_delete_user_request(request_t *req, int client_socket)
{
    printf("DELETE /user request ricevuta...\n");
    // Aggiungi qui il codice per gestire la richiesta DELETE
    print_request(req);

    // liberare la memoria della request (free_request(req))
    return true;
}
