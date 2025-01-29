#include "request_handler.h"

bool handle_post_user_request(request_t *req)
{
    printf("Controllo che user sia formattato correttamente..\n");
    printf("Aggiungo nuovo user..\n");

    print_request(req);
    return true;
}

bool handle_post_film_request(request_t *req)
{
    printf("Controllo se l'utente è un NEGOZIANTE...\n");
    printf("Controllo se il film è formattato correttamente...\n");
    printf("Aggiungo nuovo film...\n");

    print_request(req);
    return true;
}

bool handle_post_message_request(request_t *req)
{
    printf("Controllo se l'utente è un NEGOZIANTE...\n");
    printf("Controllo se il messaggio è formattato correttamente...\n");
    printf("Aggiungo nuovo messaggio...\n");

    print_request(req);
    return true;
}