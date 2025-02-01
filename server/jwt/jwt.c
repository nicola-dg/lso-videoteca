#include "jwt.h"

jwt_payload_t *init_jwt_payload(char *user_id, char *user_role)
{
    jwt_payload_t *payload = (jwt_payload_t *)malloc(sizeof(jwt_payload_t));

    strncpy(payload->user_id, user_id, MAX_USER_ID_LEN - 1);
    payload->user_id[MAX_USER_ID_LEN - 1] = '\0';

    strncpy(payload->user_role, user_role, MAX_USER_ROLE_LEN - 1);
    payload->user_role[MAX_USER_ROLE_LEN - 1] = '\0';

    return payload;
}

jwt_t *init_jwt(jwt_payload_t *payload)
{
    jwt_t *jwt;
    char *token;

    time_t current_time = time(NULL); // Ottieni il tempo attuale
    time_t expiry_time = current_time + 3600;
    char expiry_str[20];
    snprintf(expiry_str, sizeof(expiry_str), "%ld", expiry_time);

    // Creazione del JWT
    jwt_new(&jwt);
    jwt_add_grant(jwt, "user_id", payload->user_id);
    jwt_add_grant(jwt, "user_role", payload->user_role);
    jwt_add_grant(jwt, "exp", expiry_str);
    jwt_set_alg(jwt, JWT_ALG_HS256, (unsigned char *)VERY_SERCRET_KEY, strlen(VERY_SERCRET_KEY));

    // Generazione del token
    token = jwt_encode_str(jwt);
    printf("JWT: %s\n", token);

    // Pulizia memoria
    free(token);
    return jwt;
}

// Funzione per verificare la scadenza del JWT
bool is_jwt_expired(jwt_t *jwt)
{
    time_t current_time = time(NULL); // Ottieni il tempo corrente
    char expiry_str[20];

    strcpy(expiry_str, jwt_get_grant(jwt, "exp"));
    time_t exp_time = (time_t)strtol(expiry_str, NULL, 10);
    snprintf(expiry_str, 20, "%ld", exp_time);

    // Confronta la scadenza con il tempo corrente
    if (current_time > exp_time)
    {
        printf("Il JWT è scaduto.\n");
        return true; // JWT scaduto
    }
    else
    {
        printf("Il JWT è valido.\n");
        return false; // JWT valido
    }
}

// Funzione per decodificare un JWT
bool decode_jwt(const char *token)
{
    jwt_t *jwt;
    int ret;

    // Decodifica il JWT
    ret = jwt_decode(&jwt, token, (unsigned char *)VERY_SERCRET_KEY, strlen(VERY_SERCRET_KEY));
    if (ret != 0)
    {
        // Se c'è un errore nella decodifica, restituiamo il messaggio di errore
        printf("Errore durante la decodifica del JWT: %s\n", strerror(ret));
        return false;
    }

    const char *user_id = jwt_get_grant(jwt, "user_id");
    const char *user_role = jwt_get_grant(jwt, "user_role");

    printf("User ID: %s\n", user_id ? user_id : "Non presente");
    printf("User Role: %s\n", user_role ? user_role : "Non presente");

    // Libera la memoria del JWT
    jwt_free(jwt);

    return true; // JWT valido e non scaduto
}
