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
