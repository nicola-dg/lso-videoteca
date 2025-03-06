#include <jwt.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define VERY_SECRET_KEY "s_3cr3!7"
#define MAX_USER_ID_LEN 50
#define MAX_USER_ROLE_LEN 100

struct jwt_payload
{
    char user_id[MAX_USER_ID_LEN];
    char user_role[MAX_USER_ROLE_LEN];
};

typedef struct jwt_payload jwt_payload_t;

jwt_payload_t *init_jwt_payload(char *user_id, char *user_role);
jwt_t *init_jwt(jwt_payload_t *payload);
bool is_jwt_expired(jwt_t *jwt);
bool decode_jwt(const char *token);
char *jwt_extract_user_id(const char *token);
char *jwt_extract_user_role(const char *token);