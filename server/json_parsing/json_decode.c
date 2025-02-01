

#include "json.h"


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
    if (json_is_object(headers_json))
    {
        const char *key;
        json_t *value;
        json_object_foreach(headers_json, key, value)
        {
            if (strlen(key) < MAX_KEY_LEN && json_is_string(value))
            {
                struct header new_header;
                strncpy(new_header.key, key, MAX_KEY_LEN);
                strncpy(new_header.value, json_string_value(value), MAX_VALUE_LEN);

                // Print the header for debugging
                printf("Header: %s -> %s\n", new_header.key, new_header.value);

                for (int i = 0; i < MAX_HEADERS; i++)
                {
                    if (req->headers->headerCollection[i].key[0] == '\0')
                    {
                        req->headers->headerCollection[i] = new_header;
                        break;
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