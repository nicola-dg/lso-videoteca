#include "request_print.h"

// Funzione per stampare gli headers
void print_headers(const headers_t *headers) {
    printf("Headers:\n");
    for (int i = 0; i < MAX_HEADERS; i++) {
        if (headers->headerCollection[i].key[0] != '\0') {
            printf("  %s: %s\n", headers->headerCollection[i].key, headers->headerCollection[i].value);
        }
    }
}

// Funzione per stampare i parametri
void print_params(const params_t *params) {
    printf("Params:\n");
    for (int i = 0; i < MAX_HEADERS; i++) {
        if (params->paramCollection[i].key[0] != '\0') {
            printf("  %s: %s\n", params->paramCollection[i].key, params->paramCollection[i].value);
        }
    }
}

// Funzione per stampare la struttura request
void print_request(const request_t *req) {
    if (req == NULL) {
        printf("Request is NULL\n");
        return;
    }

    printf("Request Details:\n");

    // Stampa del metodo
    if (req->method != NULL) {
        printf("  Method: %s\n", req->method);
    } else {
        printf("  Method: NULL\n");
    }

    // Stampa del percorso
    if (req->path != NULL) {
        printf("  Path: %s\n", req->path);
    } else {
        printf("  Path: NULL\n");
    }

    // Stampa del payload
    if (req->payload != NULL) {
        printf("  Payload: %s\n", req->payload);
    } else {
        printf("  Payload: NULL\n");
    }

    // Stampa degli headers
    if (req->headers != NULL) {
        print_headers(req->headers);
    } else {
        printf("  Headers: NULL\n");
    }

    // Stampa dei parametri
    if (req->params != NULL) {
        print_params(req->params);
    } else {
        printf("  Params: NULL\n");
    }
}
