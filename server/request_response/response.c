
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

    // Serialize response
    char *json_string = serialize_response(res);
    if (!json_string)
        return false;

    // Append newline
    char *json_with_newline = malloc(strlen(json_string) + 2); // +2 for '\n' and '\0'
    if (!json_with_newline)
    {
        free(json_string);
        return false;
    }
    sprintf(json_with_newline, "%s\n", json_string);
    free(json_string);
    json_string = json_with_newline;

    // Ensure full transmission
    size_t total_sent = 0;
    size_t to_send = strlen(json_string);
    while (total_sent < to_send)
    {
        ssize_t bytes = send(client_socket, json_string + total_sent, to_send - total_sent, 0);
        if (bytes <= 0)
        {
            free(json_string);
            return false; // Error sending data
        }
        total_sent += bytes;
    }

    free(json_string);
    return true;
}
