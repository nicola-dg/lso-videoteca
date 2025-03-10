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
    for (int i = 0; req->headers->headerCollection[i].key != NULL; i++)
    {
        // Verifica se l'header è 'Authorization'
        if (strcmp(req->headers->headerCollection[i].key, "Authorization") == 0)
        {
            // Se trovi l'header Authorization, estrai il valore (che è il JWT)
            char *auth_header = req->headers->headerCollection[i].value;

            // Controlla se l'header contiene 'Bearer ' e rimuovilo, lasciando solo il token
            if (strncmp(auth_header, "Bearer ", 7) == 0)
            {
                // Calcola la lunghezza del token
                size_t token_length = strlen(auth_header + 7);

                // Alloca memoria per una copia del token
                char *jwt_token = (char *)malloc(token_length + 1); // +1 per il terminatore null
                if (!jwt_token)
                {
                    printf("Errore di allocazione memoria per il JWT.\n");
                    return NULL;
                }

                // Copia il token nella memoria appena allocata
                strncpy(jwt_token, auth_header + 7, token_length);
                jwt_token[token_length] = '\0'; // Assicura la terminazione della stringa

                return jwt_token; // Restituisci il token copiato
            }
        }
    }

    return NULL;
}

bool isUser(request_t *req)
{
    char *jwt_token = extract_jwt_from_headers(req);
    if (!jwt_token)
    {
        return false;
    }

    jwt_t *jwt = decode_jwt(jwt_token);
    if (!jwt)
    {
        return false;
    }

    char *user_role = jwt_extract_user_role(jwt);
    if (!user_role)
    {
        jwt_free(jwt);
        return false;
    }

    bool result = (strcmp(user_role, "USER") == 0);

    jwt_free(jwt);
    free(user_role);
    return result;
}

bool isNegoziante(request_t *req)
{
    char *jwt_token = extract_jwt_from_headers(req);
    if (!jwt_token)
    {
        return false; // Nessun token, quindi l'utente non è negoziante
    }

    jwt_t *jwt = decode_jwt(jwt_token);
    if (!jwt)
    {
        return false; // JWT non valido o decodifica fallita
    }

    char *user_role = jwt_extract_user_role(jwt);
    if (!user_role)
    {
        jwt_free(jwt);
        return false; // Ruolo non trovato, quindi non è negoziante
    }

    printf("user role :%s\n", user_role);

    bool result = (strcmp(user_role, "NEGOZIANTE") == 0);

    jwt_free(jwt);
    free(user_role);
    return result;
}
