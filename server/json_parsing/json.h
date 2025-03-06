#pragma once
#include <jansson.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../types/request_type.h"
#include "../types/user_type.h"
#include "../types/film_type.h"
#include "../request_response/request.h"

// Funzioni per l'estrazione dei campi JSON dalla richiesta
bool extract_method(json_t *root, request_t *req);
bool extract_path(json_t *root, request_t *req);
bool extract_payload(json_t *root, request_t *req);
bool extract_headers(json_t *root, request_t *req);
bool extract_params(json_t *root, request_t *req);

// Funzione per decodificare la stringa JSON in una request_t
request_t *decode(char *json_str);

user_t* extract_user_from_json(char *root);
film_t *extract_film_from_json(char *json_payload);