
#include "response.h"

response_t *init_response()
{
    response_t *response = (response_t *)malloc(sizeof(response_t));
    return response;
}

void free_response(response_t *res)
{
    if (res)
    {
        free(res);
    }
}

char *serialize_response(response_t *res)
{
    json_t *json_response = json_object(); // Crea un oggetto JSON
    json_object_set_new(json_response, "Status Code", json_string(res->status_code));
    json_object_set_new(json_response, "Phrase", json_string(res->phrase));
    json_object_set_new(json_response, "Payload", json_string(res->payload));

    // Serializza l'oggetto JSON in una stringa
    char *json_string = json_dumps(json_response, JSON_ENCODE_ANY);

    json_decref(json_response); // Decrementa il riferimento all'oggetto JSON
    return json_string;
}

bool send_response(response_t *res, int client_socket)
{
    if (!res)
        return false;

    // Serializza la risposta in formato JSON
    char *json_string = serialize_response(res);

    if (!json_string)
    {
        return false;
    }

    // Invia la stringa JSON attraverso la socket
    pthread_t tid = pthread_self();
    printf("sending response from thread-%ld to client-%d\n", tid, client_socket);
    size_t bytes_sent = send(client_socket, json_string, strlen(json_string), 0);

    free(json_string);                        // Libera la memoria allocata per la stringa JSON
    return bytes_sent == strlen(json_string); // Verifica che siano stati inviati tutti i byte
}
