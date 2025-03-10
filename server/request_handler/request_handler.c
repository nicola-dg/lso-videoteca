#include "request_handler.h"

/*------------------------------------------------------------------*/
/*-------------------------- POST REQUESTS -------------------------*/
/*------------------------------------------------------------------*/

bool handle_post_user_request(request_t *req, int client_socket)
{
    response_t *res = init_response();

    user_t *user = extract_user_from_json(req->payload);

    if (is_valid_user(user) && insert_user(user->username, user->password, user->email, user->name, user->surname, user->role))
    {
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
    response_t *res = init_response();
    // jwt_t *jwt = decode_jwt(extract_jwt_from_headers(req));
    // if (is_jwt_expired(jwt))
    // {
    //     strcpy(res->status_code, "401");
    //     strcpy(res->phrase, "Jwt Expired");
    // }
    // else
    // {
    // char *role = jwt_extract_user_role(jwt);
    // if (strcmp(role, "USER") == 0)
    // { // DOVRA' CONTROLLARE CHE SIA NEGOZIANTE METTO USER PER TESTARE
    film_t *film = extract_film_from_json(req->payload);
    if (insert_film(film->title, film->genre, film->price))
    {
        strcpy(res->status_code, "200");
        strcpy(res->phrase, "ok");
        strcpy(res->payload, req->payload);
    }
    else
    {
        strcpy(res->status_code, "500");
        strcpy(res->phrase, "Server Error");
    }
    //}
    // else
    //{
    //    strcpy(res->status_code, "400");
    //    strcpy(res->phrase, "Permission Denied");
    //}
    //}

    send_response(res, client_socket);
    free_request(req);
    free_response(res);
    // jwt_free(jwt);
    return true;
}

bool handle_post_message_request(request_t *req, int client_socket)
{
    printf("POST /message request ricevuta...\n");
    print_request(req);
    response_t *res = init_response();
    printf("Controllo se l'utente è un NEGOZIANTE...\n");
    // if (isUser(req)) // TODO: RIMUOVERE COMMENTI
    // {
    message_t *message = extract_message_from_json(req->payload);

    if (insert_message("1", message->text)) // TODO: USER ID VA ESTRATTO DAL JWT
    {
        free(message);
        strcpy(res->status_code, "200");
        strcpy(res->phrase, "ok");
    }
    else
    {
        free(message);
        strcpy(res->status_code, "400");
        strcpy(res->phrase, "Can't add to cart");
    }
    //}
    // else
    // {
    //     strcpy(res->status_code, "402");
    //     strcpy(res->phrase, "Don't have permissions");
    // }

    send_response(res, client_socket);
    free_request(req);
    free_response(res);
    return true;
}

bool handle_post_cart_film_request(request_t *req, int client_socket)
{
    printf("POST /cart/film request ricevuta...\n");
    print_request(req);
    response_t *res = init_response();
    printf("Controllo se l'utente è un USER...\n");
    // if (isUser(req)) // TODO: RIMUOVERE COMMENTI
    // {
    film_t *film = extract_film_from_json(req->payload);
    char *film_id = malloc(sizeof(char) * 100);
    select_film_id_by_title(film->title, film_id);
    if (!film_id)
    {
        return false;
    }
    if (insert_cart(film_id, "1")) // TODO: USER ID VA ESTRATTO DAL JWT
    {
        free(film);
        free(film_id);
        strcpy(res->status_code, "200");
        strcpy(res->phrase, "ok");
    }
    else
    {
        free(film);
        free(film_id);
        strcpy(res->status_code, "400");
        strcpy(res->phrase, "Can't add to cart");
    }
    //}
    // else
    // {
    //     strcpy(res->status_code, "402");
    //     strcpy(res->phrase, "Don't have permissions");
    // }

    send_response(res, client_socket);
    free_request(req);
    free_response(res);
    return true;
}

bool handle_post_loan_film_request(request_t *req, int client_socket)
{
    printf("Controllo se l'utente è un USER...\n");
    print_request(req);
    response_t *res = init_response();
    printf("POST /loan/film request ricevuta...\n");

    // if (isUser(req)) // TODO: RIMUOVERE COMMENTI
    // {
    film_t *film = extract_film_from_json(req->payload);

    if (insert_loan(film->id, "1")) // TODO: USER ID VA ESTRATTO DAL JWT
    {
        free(film);
        strcpy(res->status_code, "200");
        strcpy(res->phrase, "ok");
    }
    else
    {
        free(film);
        strcpy(res->status_code, "400");
        strcpy(res->phrase, "Can't add to cart");
    }
    //}
    // else
    // {
    //     strcpy(res->status_code, "402");
    //     strcpy(res->phrase, "Don't have permissions");
    // }

    send_response(res, client_socket);
    free_request(req);
    free_response(res);
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
    select_user_id_by_username(user->username, user->id);

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
    print_request(req);

    response_t *res = init_response();

    // jwt_t *jwt = decode_jwt(extract_jwt_from_headers(req));

    // const char *user_id = jwt_extract_user_id(jwt);
    // const char *user_role = jwt_extract_user_role(jwt);

    // printf("user_id: %s, user_role:%s\n", user_id, user_role);

    char *films = select_all_films();

    if (films == NULL)
    {
        // Errore del server nel recupero dei film
        strcpy(res->status_code, "500");
        strcpy(res->phrase, "Server Error");
        strcpy(res->payload, " ");
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
    }

    send_response(res, client_socket);

    free_request(req);
    free_response(res);
    free(films); // Libera la memoria allocata per la stringa dei film

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
    printf("GET /loan/expire request ricevuta...\n");
    print_request(req);

    response_t *res = init_response();

    // TODO: CONTROLLA CHE SIA USER
    char *expired_loans = select_all_expired_loans();

    if (expired_loans == NULL)
    {
        strcpy(res->status_code, "500");
        strcpy(res->phrase, "Server Error");
        strcpy(res->payload, " ");
    }
    else if (strlen(expired_loans) == 0)
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
        strcpy(res->payload, expired_loans);
    }

    send_response(res, client_socket);

    free_request(req);
    free_response(res);
    free(expired_loans); // Libera la memoria allocata per la stringa dei film

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

bool handle_get_loan_request(request_t *req, int client_socket)
{
    printf("GET /loan request ricevuta...\n");
    print_request(req);

    response_t *res = init_response();

    // TODO: CONTROLLA CHE SIA USER
    char *loans = select_active_loans_by_id("1"); // TODO: USERID VA RECUPERATO DAL JWT

    if (loans == NULL)
    {
        strcpy(res->status_code, "500");
        strcpy(res->phrase, "Server Error");
        strcpy(res->payload, " ");
    }
    else if (strlen(loans) == 0)
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
        strcpy(res->payload, loans);
    }

    send_response(res, client_socket);

    free_request(req);
    free_response(res);
    free(loans); // Libera la memoria allocata per la stringa dei film

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
    print_request(req);
    response_t *res = init_response();
    printf("controllo che sia un USER...\n");

    // if (isUser(req)) // TODO: RIMUOVERE COMMENTI
    // {
    film_t *film = extract_film_from_json(req->payload);
    if (update_film_return(film->id, "1")) // TODO: USER ID VA ESTRATTO DAL JWT
    {
        free(film);
        strcpy(res->status_code, "200");
        strcpy(res->phrase, "ok");
    }
    else
    {
        free(film);
        strcpy(res->status_code, "400");
        strcpy(res->phrase, "Can't add to cart");
    }
    //}
    // else
    // {
    //     strcpy(res->status_code, "402");
    //     strcpy(res->phrase, "Don't have permissions");
    // }

    send_response(res, client_socket);
    free_request(req);
    free_response(res);
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
