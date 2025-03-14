

#include "json.h"

/*--------------------------------------------------*/
/*-----------------REQUEST DECODING-----------------*/
/*--------------------------------------------------*/

bool extract_method(json_t *root, request_t *req)
{
    json_t *method_json = json_object_get(root, "method");
    if (json_is_string(method_json))
    {
        strncpy(req->method, json_string_value(method_json), MAX_KEY_LEN);
        return true;
    }
    return false;
}

bool extract_path(json_t *root, request_t *req)
{
    json_t *path_json = json_object_get(root, "path");
    if (json_is_string(path_json))
    {
        strncpy(req->path, json_string_value(path_json), MAX_VALUE_LEN);
        return true;
    }
    return false;
}

bool extract_payload(json_t *root, request_t *req)
{
    json_t *payload_json = json_object_get(root, "payload");
    if (json_is_string(payload_json))
    {
        strncpy(req->payload, json_string_value(payload_json), MAX_VALUE_LEN);
        return true;
    }
    return false;
}

bool extract_headers(json_t *root, request_t *req)
{
    json_t *headers_json = json_object_get(root, "headers");
    if (json_is_array(headers_json)) // Verifica che 'headers' sia un array
    {
        size_t index;
        json_t *header_json;

        // Itera sull'array di header
        json_array_foreach(headers_json, index, header_json)
        {
            if (json_is_object(header_json)) // Verifica che ogni elemento dell'array sia un oggetto
            {
                const char *key;
                json_t *value;

                // Itera sull'oggetto dell'header (assumendo che sia una coppia chiave-valore)
                json_object_foreach(header_json, key, value)
                {
                    if (strlen(key) < MAX_KEY_LEN && json_is_string(value))
                    {
                        struct header new_header;
                        strncpy(new_header.key, key, MAX_KEY_LEN);
                        strncpy(new_header.value, json_string_value(value), MAX_VALUE_LEN);

                        // Aggiungi l'header all'array di header di 'req'
                        for (int i = 0; i < MAX_HEADERS; i++)
                        {
                            if (req->headers->headerCollection[i].key[0] == '\0') // Se trovi uno spazio vuoto
                            {
                                req->headers->headerCollection[i] = new_header;
                                break;
                            }
                        }
                    }
                }
            }
        }
        return true;
    }
    return false;
}

bool extract_params(json_t *root, request_t *req)
{
    json_t *params_json = json_object_get(root, "params");
    if (json_is_object(params_json))
    {
        const char *key;
        json_t *value;
        json_object_foreach(params_json, key, value)
        {
            if (strlen(key) < MAX_KEY_LEN && json_is_string(value))
            {
                struct param new_param;
                strncpy(new_param.key, key, MAX_KEY_LEN);
                strncpy(new_param.value, json_string_value(value), MAX_VALUE_LEN);

                for (int i = 0; i < MAX_HEADERS; i++)
                {
                    if (req->params->paramCollection[i].key[0] == '\0')
                    {
                        req->params->paramCollection[i] = new_param;
                        break;
                    }
                }
            }
        }
        return true;
    }
    return false;
}

request_t *decode(char *json_str)
{
    // Creare un oggetto request_t
    request_t *req = init_request();
    if (req == NULL)
    {
        printf("passing null to decode()\n");
        return NULL;
    }

    // Parse del JSON
    json_t *root;
    json_error_t error;
    root = json_loads(json_str, 0, &error); // Carica il JSON dalla stringa

    if (root == NULL)
    {
        printf("Se il parsing fallisce, libera la memoria allocata e ritorna NULL\n");
        free_request(req);
        return NULL;
    }

    extract_method(root, req);
    extract_path(root, req);
    extract_payload(root, req);
    extract_headers(root, req);
    extract_params(root, req);

    // Libera la memoria del JSON una volta che l'analisi è completata
    json_decref(root);

    return req;
}

/*--------------------------------------------------*/
/*-------------------USER DECODING------------------*/
/*--------------------------------------------------*/

user_t *extract_user_from_json(char *json_payload)
{
    if (!json_payload)
        return NULL; // Controllo parametri nulli

    json_t *root;
    json_error_t error;

    root = json_loads(json_payload, 0, &error);
    if (!root)
    {
        fprintf(stderr, "Errore nel parsing JSON: %s\n", error.text);
        return NULL;
    }

    user_t *user = (user_t *)malloc(sizeof(user_t));

    const char *value;
    if ((value = json_string_value(json_object_get(root, "id"))))
        snprintf(user->id, sizeof(user->id), "%s", value);

    if ((value = json_string_value(json_object_get(root, "username"))))
        snprintf(user->username, sizeof(user->username), "%s", value);

    if ((value = json_string_value(json_object_get(root, "password"))))
        snprintf(user->password, sizeof(user->password), "%s", value);

    if ((value = json_string_value(json_object_get(root, "email"))))
        snprintf(user->email, sizeof(user->email), "%s", value);

    if ((value = json_string_value(json_object_get(root, "name"))))
        snprintf(user->name, sizeof(user->name), "%s", value);

    if ((value = json_string_value(json_object_get(root, "surname"))))
        snprintf(user->surname, sizeof(user->surname), "%s", value);

    if ((value = json_string_value(json_object_get(root, "role"))))
        snprintf(user->role, sizeof(user->role), "%s", value);

    if ((value = json_string_value(json_object_get(root, "max_loans"))))
        snprintf(user->max_loans, sizeof(user->max_loans), "%s", value);

    json_decref(root); // Libera la memoria allocata da jansson

    return user;
}

/*--------------------------------------------------*/
/*-------------------FILM DECODING------------------*/
/*--------------------------------------------------*/

film_t *extract_film_from_json(char *json_payload)
{
    if (!json_payload)
        return NULL; // Controllo parametri nulli

    json_t *root;
    json_error_t error;

    root = json_loads(json_payload, 0, &error);
    if (!root)
    {
        fprintf(stderr, "Errore nel parsing JSON: %s\n", error.text);
        return NULL;
    }

    film_t *film = (film_t *)malloc(sizeof(film_t));

    const char *value;
    if ((value = json_string_value(json_object_get(root, "id"))))
        snprintf(film->id, sizeof(film->id), "%s", value);

    if ((value = json_string_value(json_object_get(root, "title"))))
        snprintf(film->title, sizeof(film->title), "%s", value);

    if ((value = json_string_value(json_object_get(root, "genre"))))
        snprintf(film->genre, sizeof(film->genre), "%s", value);

    if ((value = json_string_value(json_object_get(root, "total_copies"))))
        snprintf(film->total_copies, sizeof(film->total_copies), "%s", value);

    if ((value = json_string_value(json_object_get(root, "available_copies"))))
        snprintf(film->available_copies, sizeof(film->available_copies), "%s", value);

    if ((value = json_string_value(json_object_get(root, "price"))))
        snprintf(film->price, sizeof(film->price), "%s", value);

    if ((value = json_string_value(json_object_get(root, "loan_count"))))
        snprintf(film->loan_count, sizeof(film->loan_count), "%s", value);

    json_decref(root); // Libera la memoria allocata da Jansson

    return film;
}

/*--------------------------------------------------*/
/*-------------------MESSAGE DECODING------------------*/
/*--------------------------------------------------*/

message_t *extract_message_from_json(char *json_payload)
{
    if (!json_payload)
        return NULL; // Controllo parametri nulli

    json_t *root;
    json_error_t error;

    root = json_loads(json_payload, 0, &error);
    if (!root)
    {
        fprintf(stderr, "Errore nel parsing JSON: %s\n", error.text);
        return NULL;
    }

    message_t *message = (message_t *)malloc(sizeof(message_t));

    const char *value;
    if ((value = json_string_value(json_object_get(root, "id"))))
        snprintf(message->id, sizeof(message->id), "%s", value);

    if ((value = json_string_value(json_object_get(root, "user_id"))))
        snprintf(message->user_id, sizeof(message->user_id), "%s", value);

    if ((value = json_string_value(json_object_get(root, "text"))))
        snprintf(message->text, sizeof(message->text), "%s", value);

    if ((value = json_string_value(json_object_get(root, "checkout_date"))))
        snprintf(message->checkout_date, sizeof(message->checkout_date), "%s", value);

    json_decref(root); // Libera la memoria allocata da Jansson

    return message;
}