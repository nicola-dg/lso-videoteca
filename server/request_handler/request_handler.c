#include "request_handler.h"

/*------------------------------------------------------------------*/
/*-------------------------- POST REQUESTS -------------------------*/
/*------------------------------------------------------------------*/

bool handle_post_user_request(request_t *req, int client_socket)
{
    printf("Controllo che user sia formattato correttamente..\n");
    printf("Aggiungo nuovo user..\n");

    user_t *user = extract_user_from_json(req->payload);
    // Aggiungi qui il codice per gestire la richiesta POST
    print_request(req);

    printf("username->%s\n", user->username);
    printf("name->%s\n", user->name);
    printf("surname->%s\n", user->surname);
    printf("email->%s\n", user->email);
    printf("password->%s\n", user->password);

    insert_user(user->name, user->password, user->email, user->name, user->surname);

    response_t *res = init_response();
    strcpy(res->status_code, "200");
    strcpy(res->phrase, "Ok");
    strcpy(res->payload, req->payload);
    send_response(res, client_socket);

    free_request(req);
    free_response(res);

    // liberare la memoria della request (free_request(req))
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
    printf("GET /user request ricevuta...\n");
    // Aggiungi qui il codice per gestire la richiesta GET
    // print_request(req);

    // user_t user = init_user(req)
    // jwt_payload = login_user(user_t)
    /*
        supposto di aver un payload recuperato dal db crea il jwt
    */
    jwt_payload_t *payload = init_jwt_payload("12", "NEGOZIANTE");
    jwt_t *jwt = init_jwt(payload);

    response_t *res = init_response();
    strcpy(res->status_code, "200");
    strcpy(res->phrase, "Ok");
    strcpy(res->payload, jwt_encode_str(jwt));
    send_response(res, client_socket);

    jwt_free(jwt);
    free_request(req);
    free_response(res);

    return true;
}

bool handle_get_film_request(request_t *req, int client_socket)
{
    printf("GET /film request ricevuta...\n");
    // Aggiungi qui il codice per gestire la richiesta GET
    print_request(req);

    // liberare la memoria della request (free_request(req))
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
