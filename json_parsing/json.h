#pragma once
#include <jansson.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_KEY_LEN 100
#define MAX_VALUE_LEN 500
#define MAX_HEADERS 100
#define MAX_PARAMS 100

// Strutture per headers e parametri della richiesta
struct header
{
    char key[MAX_KEY_LEN];
    char value[MAX_VALUE_LEN];
};

struct headers
{
    struct header headerCollection[MAX_HEADERS];
};

typedef struct headers headers_t;

struct param
{
    char key[MAX_KEY_LEN];
    char value[MAX_VALUE_LEN];
};

struct params
{
    struct param paramCollection[MAX_PARAMS];
};

typedef struct params params_t;

struct request
{
    headers_t *headers;
    char *method;
    char *path;
    params_t *params;
    char *payload;
};

typedef struct request request_t;

// Funzioni per la gestione della richiesta
request_t *initRequest();
void free_request(request_t *req);

// Funzioni per l'estrazione dei campi JSON dalla richiesta
bool extract_method(json_t *root, request_t *req);
bool extract_path(json_t *root, request_t *req);
bool extract_payload(json_t *root, request_t *req);
bool extract_headers(json_t *root, request_t *req);
bool extract_params(json_t *root, request_t *req);

// Funzione per decodificare la stringa JSON in una request_t
request_t *decode(char *json_str);