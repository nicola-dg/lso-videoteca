#include "request.h"

request_t *init_request()
{
    request_t *req = (request_t *)malloc(sizeof(request_t));
    if (req == NULL)
    {
        printf("errore durante una malloc in init_request()\n");
        return NULL;
    }

    req->headers = (headers_t *)malloc(sizeof(headers_t));
    req->headers->headerCollection->key[0] = '\0';
    req->params = (params_t *)malloc(sizeof(params_t));
    req->params->paramCollection->key[0] = '\0';
    // Assicurati che le stringhe siano allocate
    req->method = (char *)malloc(MAX_KEY_LEN * sizeof(char));
    req->path = (char *)malloc(MAX_VALUE_LEN * sizeof(char));
    req->payload = (char *)malloc(MAX_VALUE_LEN * sizeof(char));

    if (req->headers == NULL || req->params == NULL || req->method == NULL || req->path == NULL || req->payload == NULL)
    {
        free(req);
        printf("errore durante una malloc in init_request()\n");
        return NULL;
    }

    return req;
}

void free_request(request_t *req)
{
    if (req)
    {
        free(req->headers);
        free(req->params);
        free(req->method);
        free(req->path);
        free(req->payload);
        free(req);
    }
}

bool is_valid_user(user_t *user)
{
    if (!user)
        return false;
    return (user->name && user->surname && user->username && user->email && user->password && user->role);
}


// Funzione per estrarre il JWT dal campo Authorization degli header
char *extract_jwt_from_headers(request_t *req)
{
    // Scorri tutti gli header fino a trovare NULL
    for (int i = 0; req->headers->headerCollection[i].key != NULL; i++) {
        // Verifica se l'header è 'Authorization'
        if (strcmp(req->headers->headerCollection[i].key, "Authorization") == 0) {
            // Se trovi l'header Authorization, estrai il valore (che è il JWT)
            char *auth_header = req->headers->headerCollection[i].value;

            // Controlla se l'header contiene 'Bearer' e rimuovilo, lasciando solo il token
            if (strncmp(auth_header, "Bearer ", 7) == 0) {
                // Restituisci solo la parte del token dopo "Bearer "
                return auth_header + 7;
            }
        }
    }

    return NULL;
}