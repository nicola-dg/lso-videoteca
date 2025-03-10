#include "database.h"

PGconn *conn;

void database_start(bool reset)
{
    database_connect();
    if (reset)
    {
        reset_tables();
    }
    create_tables();
    prepare_insert_statements();
    prepare_update_statements();
    prepare_delete_statements();
    prepare_select_statements();
}

void database_connect()
{

    conn = PQconnectdb("host=localhost port=5432 dbname=videotecadb user=postgres password=1234");
    if (PQstatus(conn) != CONNECTION_OK)
    {
        fprintf(stderr, "Errore di connessione: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
    }
    else
    {
        printf("Connessione al database riuscita!\n");
    }
}

void reset_tables()
{
    const char *queries[] = {
        "TRUNCATE TABLE carts RESTART IDENTITY CASCADE;",
        "TRUNCATE TABLE loans RESTART IDENTITY CASCADE;",
        "TRUNCATE TABLE films RESTART IDENTITY CASCADE;",
        "TRUNCATE TABLE users RESTART IDENTITY CASCADE;"};

    size_t num_queries = sizeof(queries) / sizeof(queries[0]);

    for (size_t i = 0; i < num_queries; i++)
    {
        PGresult *res = PQexec(conn, queries[i]);

        if (PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            fprintf(stderr, "Errore reset tabella %zu: %s\n", i, PQerrorMessage(conn));
        }
        else
        {
            printf("Tabella %zu resettata con successo!\n", i);
        }

        PQclear(res);
    }
}

void create_tables()
{
    const char *queries[] = {
        "CREATE TABLE IF NOT EXISTS users ("
        "id SERIAL PRIMARY KEY, "
        "username TEXT UNIQUE, "
        "password TEXT, "
        "email TEXT, "
        "name TEXT, "
        "surname TEXT, "
        "role TEXT, "
        "max_loans INTEGER DEFAULT 3);",

        "CREATE TABLE IF NOT EXISTS films ("
        "id SERIAL PRIMARY KEY, "
        "title TEXT NOT NULL UNIQUE, "
        "genre TEXT, "
        "price NUMERIC(10,2) DEFAULT 0.00, "
        "total_copies INTEGER DEFAULT 1, "
        "available_copies INTEGER DEFAULT 1, "
        "loan_count INTEGER DEFAULT 0);",

        "CREATE TABLE IF NOT EXISTS loans ("
        "id SERIAL PRIMARY KEY, "
        "film_id INTEGER REFERENCES films(id) ON DELETE CASCADE, "
        "user_id INTEGER REFERENCES users(id) ON DELETE CASCADE, "
        "checkout_date TIMESTAMP DEFAULT NOW(),"
        "due_date TIMESTAMP DEFAULT NOW() + INTERVAL '14 days', "
        "return_date TIMESTAMP);",

        "CREATE TABLE IF NOT EXISTS carts ("
        "id SERIAL PRIMARY KEY, "
        "film_id INTEGER REFERENCES films(id) ON DELETE CASCADE, "
        "user_id INTEGER REFERENCES users(id) ON DELETE CASCADE, "
        "checkout_date TIMESTAMP DEFAULT NOW());"};

    size_t num_queries = sizeof(queries) / sizeof(queries[0]);

    for (size_t i = 0; i < num_queries; i++)
    {
        PGresult *res = PQexec(conn, queries[i]);

        if (PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            fprintf(stderr, "Errore creazione tabella %zu: %s\n", i, PQerrorMessage(conn));
        }
        else
        {
            printf("Tabella %zu creata con successo!\n", i);
        }

        PQclear(res);
    }
}

bool execute_prepared_statement(const char *stmt_name, int nParams, const char **paramValues)
{

    bool ret;
    // Esegui la query preparata con i parametri passati
    PGresult *res = PQexecPrepared(conn, stmt_name, nParams, paramValues, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK && PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        // Se la query non va a buon fine, stampa l'errore
        fprintf(stderr, "Errore nell'esecuzione della query '%s': %s\n", stmt_name, PQerrorMessage(conn));
        ret = false;
    }
    else
    {
        // Se la query è stata eseguita correttamente
        printf("Esecuzione riuscita della query '%s'.\n", stmt_name);
        ret = true;
    }

    // Pulisci il risultato della query
    PQclear(res);
    return ret;
}

void prepare_insert_statements()
{
    PGresult *res;

    // Statement per inserire un utente
    res = PQprepare(conn, "insert_user",
                    "INSERT INTO users (username, password, email, name, surname, role) VALUES ($1, crypt($2, gen_salt('bf'::text)), $3, $4, $5, $6);",
                    6, NULL);
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Errore preparazione query insert_user: %s\n", PQerrorMessage(conn));
    }
    PQclear(res);

    // Statement per inserire un film
    res = PQprepare(conn, "insert_film",
                    "INSERT INTO films (title, genre, price) VALUES ($1, $2, $3);",
                    3, NULL);
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Errore preparazione query insert_film: %s\n", PQerrorMessage(conn));
    }
    PQclear(res);

    // Statement per inserire un carrello
    res = PQprepare(conn, "insert_cart",
                    "INSERT INTO carts (film_id, user_id) VALUES ($1, $2);",
                    2, NULL);
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Errore preparazione query insert_cart: %s\n", PQerrorMessage(conn));
    }
    PQclear(res);
}

bool insert_user(const char *username, const char *password, const char *email, const char *name, const char *surname, const char *role)
{
    PGresult *res;
    const char *paramValues[6] = {username, password, email, name, surname, role};

    // Start transaction
    res = PQexec(conn, "BEGIN");
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Error starting transaction: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return false;
    }
    PQclear(res);

    // Execute insert query
    if (!execute_prepared_statement("insert_user", 6, paramValues))
    {
        PQexec(conn, "ROLLBACK");
        return false;
    }

    // Commit transaction
    res = PQexec(conn, "COMMIT");
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Error committing transaction: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return false;
    }
    PQclear(res);
    return true;
}

bool insert_film(const char *title, const char *genre, const char *price)
{
    PGresult *res;
    const char *paramValues[3] = {title, genre, price};

    res = PQexec(conn, "BEGIN");
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Error starting transaction: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return false;
    }
    PQclear(res);

    if (!execute_prepared_statement("insert_film", 3, paramValues))
    {
        PQexec(conn, "ROLLBACK");
        return false;
    }

    res = PQexec(conn, "COMMIT");
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Error committing transaction: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return false;
    }
    PQclear(res);
    return true;
}

bool insert_cart(char *film_id, char *user_id)
{
    PGresult *res;
    const char *paramValues[2] = {film_id, user_id};

    res = PQexec(conn, "BEGIN");
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Error starting transaction: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return false;
    }
    PQclear(res);

    if (!execute_prepared_statement("insert_cart", 2, paramValues))
    {
        PQexec(conn, "ROLLBACK");
        return false;
    }

    res = PQexec(conn, "COMMIT");
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Error committing transaction: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return false;
    }
    PQclear(res);
    return true;
}

bool insert_loan(char *film_id, char *user_id)
{
    PGresult *res;

    // Start transaction
    res = PQexec(conn, "BEGIN");
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Error starting transaction: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return false;
    }
    PQclear(res);

    // Query to check available copies
    const char *check_query =
        "SELECT available_copies FROM films WHERE id = $1";

    // Prepare parameter values for the query
    const char *paramValues[1] = {film_id};

    // Execute the query to check available copies
    res = PQexecParams(conn, check_query, 1, NULL, paramValues, NULL, NULL, 0);
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "Error executing check_query: %s\n", PQerrorMessage(conn));
        PQclear(res);
        PQexec(conn, "ROLLBACK");
        return false;
    }

    int available_copies = atoi(PQgetvalue(res, 0, 0)); // Get available copies from the result
    PQclear(res);                                       // Clear the result from the previous query

    // Check if there are available copies
    if (available_copies > 0)
    {
        if (can_user_loan(user_id))
        {
            // Query to remove the film from the cart for the specific user
            const char *delete_cart_query =
                "DELETE FROM carts WHERE film_id = $1 AND user_id = $2";
            const char *paramValuesCart[2] = {film_id, user_id};
            res = PQexecParams(conn, delete_cart_query, 2, NULL, paramValuesCart, NULL, NULL, 0);
            if (PQresultStatus(res) != PGRES_COMMAND_OK)
            {
                fprintf(stderr, "Error executing delete_cart_query: %s\n", PQerrorMessage(conn));
                PQclear(res);
                PQexec(conn, "ROLLBACK");
                return false;
            }
            PQclear(res); // Clear the result from the delete query

            // Query to insert the loan with film_id and user_id
            const char *insert_loan_query =
                "INSERT INTO loans (film_id, user_id) VALUES ($1, $2)";
            const char *paramValuesLoan[2] = {film_id, user_id};
            res = PQexecParams(conn, insert_loan_query, 2, NULL, paramValuesLoan, NULL, NULL, 0);
            if (PQresultStatus(res) != PGRES_COMMAND_OK)
            {
                fprintf(stderr, "Error executing insert_loan_query: %s\n", PQerrorMessage(conn));
                PQclear(res);
                PQexec(conn, "ROLLBACK");
                return false;
            }
            PQclear(res); // Clear the result from the insert query

            // Query to update the loan count
            const char *update_loan_count_query =
                "UPDATE films SET loan_count = loan_count + 1 WHERE id = $1";
            res = PQexecParams(conn, update_loan_count_query, 1, NULL, paramValues, NULL, NULL, 0);
            if (PQresultStatus(res) != PGRES_COMMAND_OK)
            {
                fprintf(stderr, "Error executing update_loan_count_query: %s\n", PQerrorMessage(conn));
                PQclear(res);
                PQexec(conn, "ROLLBACK");
                return false;
            }
            PQclear(res); // Clear the result from the loan count update query

            // Query to update the available copies
            const char *update_available_copies_query =
                "UPDATE films SET available_copies = total_copies - loan_count WHERE id = $1";
            res = PQexecParams(conn, update_available_copies_query, 1, NULL, paramValues, NULL, NULL, 0);
            if (PQresultStatus(res) != PGRES_COMMAND_OK)
            {
                fprintf(stderr, "Error executing update_available_copies_query: %s\n", PQerrorMessage(conn));
                PQclear(res);
                PQexec(conn, "ROLLBACK");
                return false;
            }
            PQclear(res); // Clear the result from the available copies update query

            // Commit the transaction if everything is successful
            res = PQexec(conn, "COMMIT");
            if (PQresultStatus(res) != PGRES_COMMAND_OK)
            {
                fprintf(stderr, "Error committing transaction: %s\n", PQerrorMessage(conn));
                PQclear(res);
                return false;
            }
            PQclear(res);
        }
        else
        {
            // If user can't loan, raise an error
            fprintf(stderr, "Error: Max loan number reached, return a loan to get availability.\n");
            PQexec(conn, "ROLLBACK");
            return false;
        }
    }
    else
    {
        // If no copies are available, raise an error
        fprintf(stderr, "Error: No copies available for film ID %s\n", film_id);
        PQexec(conn, "ROLLBACK");
        return false;
    }

    printf("Loan successfully inserted for film ID %s and user ID %s\n", film_id, user_id);
    return true;
}

void prepare_update_statements()
{
    PGresult *res;

    // Statement per aggiornare un utente
    res = PQprepare(conn, "update_user",
                    "UPDATE users SET password = $1, email = $2, name = $3, surname = $4, max_loans = $5 WHERE username = $6;",
                    6, NULL);
    PQclear(res);

    // Statement per aggiornare la disponibilità di copie di un film
    res = PQprepare(conn, "update_film",
                    "UPDATE films SET available_copies = $1 WHERE id = $2;",
                    2, NULL);
    PQclear(res);

    // Statement per aggiornare la data di restituzione di un prestito
    res = PQprepare(conn, "update_loan",
                    "UPDATE loans SET return_date = $1 WHERE id = $2;",
                    2, NULL);
    PQclear(res);

    // Statement per aggiornare la data di checkout di un carrello
    res = PQprepare(conn, "update_cart",
                    "UPDATE carts SET checkout_date = $1 WHERE id = $2;",
                    2, NULL);
    PQclear(res);

    // Statement per aggiornare la data di checkout di un carrello
    res = PQprepare(conn, "update_film_return",
                    "UPDATE loans SET return_date = NOW() WHERE film_id = $1 AND user_id = $2;",
                    2, NULL);
    PQclear(res);
}

bool update_user(const char *password, const char *email, const char *name, const char *surname, int max_loans, const char *username)
{
    const char *paramValues[6] = {password, email, name, surname, (const char *)&max_loans, username};
    return execute_prepared_statement("update_user", 6, paramValues);
}

bool update_film(int available_copies, int film_id)
{
    const char *paramValues[2] = {(const char *)&available_copies, (const char *)&film_id};
    return execute_prepared_statement("update_film", 2, paramValues);
}

bool update_film_return(char *film_id, char *user_id)
{
    PGresult *res;
    const char *paramValues[2] = {film_id, user_id};

    // Start transaction
    res = PQexec(conn, "BEGIN");
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Error starting transaction: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return false;
    }
    PQclear(res);

    // Update the return_date in the loans table
    res = PQexecParams(conn, "UPDATE loans SET return_date = NOW() WHERE film_id = $1 AND user_id = $2",
                       2, NULL, paramValues, NULL, NULL, 0);
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Error updating return date in loans table: %s\n", PQerrorMessage(conn));
        PQclear(res);
        PQexec(conn, "ROLLBACK");
        return false;
    }
    PQclear(res);

    // Update the available_copies and loan_count in the films table
    const char *update_film_query =
        "UPDATE films SET available_copies = available_copies + 1, loan_count = loan_count - 1 "
        "WHERE id = $1";
    res = PQexecParams(conn, update_film_query, 1, NULL, paramValues, NULL, NULL, 0);
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Error updating available copies and loan count in films table: %s\n", PQerrorMessage(conn));
        PQclear(res);
        PQexec(conn, "ROLLBACK");
        return false;
    }
    PQclear(res);

    // Commit the transaction if everything is successful
    res = PQexec(conn, "COMMIT");
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Error committing transaction: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return false;
    }
    PQclear(res);

    printf("Film return successfully processed for film ID %s and user ID %s\n", film_id, user_id);
    return true;
}

bool update_loan(const char *return_date, int loan_id)
{
    const char *paramValues[2] = {return_date, (const char *)&loan_id};
    return execute_prepared_statement("update_loan", 2, paramValues);
}

bool update_cart(const char *checkout_date, int cart_id)
{
    const char *paramValues[2] = {checkout_date, (const char *)&cart_id};
    return execute_prepared_statement("update_cart", 2, paramValues);
}

void prepare_delete_statements()
{
    PGresult *res;

    // Statement per eliminare un utente
    res = PQprepare(conn, "delete_user",
                    "DELETE FROM users WHERE id = $1;",
                    1, NULL);
    PQclear(res);

    // Statement per eliminare un film
    res = PQprepare(conn, "delete_film",
                    "DELETE FROM films WHERE id = $1;",
                    1, NULL);
    PQclear(res);

    // Statement per eliminare un prestito
    res = PQprepare(conn, "delete_loan",
                    "DELETE FROM loans WHERE id = $1;",
                    1, NULL);
    PQclear(res);

    // Statement per eliminare un carrello
    res = PQprepare(conn, "delete_cart",
                    "DELETE FROM carts WHERE id = $1;",
                    1, NULL);
    PQclear(res);
}

bool delete_user(int user_id)
{
    const char *paramValues[1] = {(const char *)&user_id};
    return execute_prepared_statement("delete_user", 1, paramValues);
}

bool delete_film(int film_id)
{
    const char *paramValues[1] = {(const char *)&film_id};
    return execute_prepared_statement("delete_film", 1, paramValues);
}

bool delete_loan(int loan_id)
{
    const char *paramValues[1] = {(const char *)&loan_id};
    return execute_prepared_statement("delete_loan", 1, paramValues);
}

bool delete_cart(int cart_id)
{
    const char *paramValues[1] = {(const char *)&cart_id};
    return execute_prepared_statement("delete_cart", 1, paramValues);
}

void prepare_select_statements()
{
    PGresult *res;

    // Query preparata per selezionare tutti gli utenti
    res = PQprepare(conn, "select_all_users",
                    "SELECT id, username, email, name, surname FROM users;",
                    0, NULL);
    PQclear(res);

    // Query preparata per selezionare un utente per ID
    res = PQprepare(conn, "select_user_by_id",
                    "SELECT id, username, email, name, surname FROM users WHERE id = $1;",
                    1, NULL);
    PQclear(res);

    // Query preparata per selezionare un utente per username e password
    res = PQprepare(conn, "select_user_by_username_and_password",
                    "SELECT id, username, email, name, surname FROM users WHERE username = $1 AND password = crypt($2, password);",
                    2, NULL);
    PQclear(res);

    // Query preparata per selezionare tutti i film
    res = PQprepare(conn, "select_all_films",
                    "SELECT id, title, genre, total_copies, available_copies, loan_count FROM films;",
                    0, NULL);
    PQclear(res);

    // Query preparata per ottenere user_id a partire dallo username
    res = PQprepare(conn, "select_user_id_by_username",
                    "SELECT id FROM users WHERE username = $1 ;",
                    1, NULL);
    PQclear(res);

    // Query preparata per ottenere film_id a partire dal titolo
    res = PQprepare(conn, "select_film_id_by_title",
                    "SELECT id FROM films WHERE title = $1 ;",
                    1, NULL);
    PQclear(res);

    // Query preparata per ottenere loans a partire dal id
    res = PQprepare(conn, "select_active_loans_by_id",
                    "SELECT l.id, l.film_id, l.user_id, l.checkout_date, "
                    "l.due_date, l.return_date, f.title AS film_title "
                    "FROM loans l "
                    "JOIN films f ON l.film_id = f.id "
                    "WHERE l.user_id = $1 AND l.return_date IS NULL;",
                    1, NULL);
    PQclear(res);

    // Query preparata per ottenere loans scaduti
    res = PQprepare(conn, "select_all_expired_loans",
                    "SELECT l.id, l.film_id, l.user_id, l.checkout_date, "
                    "l.due_date, l.return_date, f.title AS film_title, u.username "
                    "FROM loans l "
                    "JOIN films f ON l.film_id = f.id "
                    "JOIN users u ON l.user_id = u.id "
                    "WHERE l.return_date IS NULL AND CURRENT_DATE > l.due_date;",
                    0, NULL);
    PQclear(res);
}

void select_all_users()
{
    PGresult *res = PQexecPrepared(conn, "select_all_users", 0, NULL, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "Errore nell'esecuzione della SELECT: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return;
    }

    int rows = PQntuples(res); // Numero di righe restituite
    int cols = PQnfields(res); // Numero di colonne

    printf("Trovati %d utenti:\n", rows);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            printf("%s: %s\t", PQfname(res, j), PQgetvalue(res, i, j));
        }
        printf("\n");
    }

    PQclear(res);
}

void select_user_by_id(int user_id)
{
    char id_str[10];
    snprintf(id_str, sizeof(id_str), "%d", user_id); // Converti l'ID in stringa

    const char *paramValues[1] = {id_str};

    PGresult *res = PQexecPrepared(conn, "select_user_by_id", 1, paramValues, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "Errore nella SELECT per ID: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return;
    }

    if (PQntuples(res) == 0)
    {
        printf("Nessun utente trovato con ID %d\n", user_id);
    }
    else
    {
        printf("Utente trovato:\n");
        printf("ID: %s\n", PQgetvalue(res, 0, 0));
        printf("Username: %s\n", PQgetvalue(res, 0, 1));
        printf("Email: %s\n", PQgetvalue(res, 0, 2));
        printf("Nome: %s\n", PQgetvalue(res, 0, 3));
        printf("Cognome: %s\n", PQgetvalue(res, 0, 4));
    }

    PQclear(res);
}

bool select_user_by_username_and_password(const char *username, const char *password)
{
    const char *paramValues[2] = {username, password};

    PGresult *res = PQexecPrepared(conn, "select_user_by_username_and_password", 2, paramValues, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "Errore nella SELECT per username e password: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return false;
    }

    if (PQntuples(res) == 0)
    {
        printf("Credenziali errate o utente non trovato.\n");
        return false;
    }
    else
    {
        printf("Login riuscito! Dati dell'utente:\n");
        printf("ID: %s\n", PQgetvalue(res, 0, 0));
        printf("Username: %s\n", PQgetvalue(res, 0, 1));
        printf("Email: %s\n", PQgetvalue(res, 0, 2));
        printf("Nome: %s\n", PQgetvalue(res, 0, 3));
        printf("Cognome: %s\n", PQgetvalue(res, 0, 4));
    }

    PQclear(res);
    return true;
}

bool select_user_id_by_username(char *username, char *user_id)
{
    const char *paramValues[1] = {username}; // Passiamo solo il parametro username

    // Esegui la query preparata per ottenere l'ID utente in base allo username
    PGresult *res = PQexecPrepared(conn, "select_user_id_by_username", 1, paramValues, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        // Se c'è un errore, stampiamo un messaggio di errore e liberiamo la memoria
        fprintf(stderr, "Errore nella SELECT per username: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return false;
    }

    if (PQntuples(res) == 0)
    {
        // Se non ci sono righe nel risultato, l'utente non è stato trovato
        printf("Utente non trovato.\n");
        PQclear(res);
        return false;
    }
    else
    {
        // Se l'utente è trovato, copiamo l'ID nella variabile user_id
        strcpy(user_id, PQgetvalue(res, 0, 0)); // Assume che l'ID sia nel primo campo
        printf("ID utente: %s\n", user_id);
    }

    PQclear(res);
    return true;
}

char *select_all_films()
{
    PGresult *res = PQexecPrepared(conn, "select_all_films", 0, NULL, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "Errore nell'esecuzione della SELECT: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return NULL;
    }

    int rows = PQntuples(res);         // Numero di righe restituite
    json_t *film_array = json_array(); // Array JSON per contenere i film

    for (int i = 0; i < rows; i++)
    {
        json_t *film_json = json_object(); // Oggetto JSON per un singolo film

        json_object_set_new(film_json, "id", json_string(PQgetvalue(res, i, 0)));
        json_object_set_new(film_json, "title", json_string(PQgetvalue(res, i, 1)));
        json_object_set_new(film_json, "genre", json_string(PQgetvalue(res, i, 2)));
        json_object_set_new(film_json, "total_copies", json_string(PQgetvalue(res, i, 3)));
        json_object_set_new(film_json, "available_copies", json_string(PQgetvalue(res, i, 4)));
        json_object_set_new(film_json, "loan_count", json_string(PQgetvalue(res, i, 5)));

        json_array_append_new(film_array, film_json); // Aggiunge il film all'array JSON
    }

    PQclear(res); // Libera il risultato della query

    // Converti il JSON in una stringa (il chiamante deve liberare la memoria con `free`)
    char *json_string_result = json_dumps(film_array, JSON_INDENT(4));
    json_decref(film_array); // Libera la struttura JSON (ma non la stringa)

    return json_string_result;
}

char *select_active_loans_by_id(char *user_id)
{
    // Parametri per la query preparata: user_id
    const char *paramValues[1] = {user_id};

    // Esegui la query preparata passando il parametro user_id
    PGresult *res = PQexecPrepared(conn, "select_active_loans_by_id", 1, paramValues, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "Errore nell'esecuzione della SELECT: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return NULL;
    }

    int rows = PQntuples(res);         // Numero di righe restituite
    json_t *loan_array = json_array(); // Array JSON per contenere i film

    for (int i = 0; i < rows; i++)
    {
        json_t *loan_json = json_object(); // Oggetto JSON per un singolo film

        json_object_set_new(loan_json, "id", json_string(PQgetvalue(res, i, 0)));
        json_object_set_new(loan_json, "film_id", json_string(PQgetvalue(res, i, 1)));
        json_object_set_new(loan_json, "user_id", json_string(PQgetvalue(res, i, 2)));
        json_object_set_new(loan_json, "checkout_date", json_string(PQgetvalue(res, i, 3)));
        json_object_set_new(loan_json, "due_date", json_string(PQgetvalue(res, i, 4)));
        json_object_set_new(loan_json, "return_date", json_string(PQgetvalue(res, i, 5)));
        json_object_set_new(loan_json, "film_title", json_string(PQgetvalue(res, i, 6)));

        json_array_append_new(loan_array, loan_json); // Aggiunge il film all'array JSON
    }

    PQclear(res); // Libera il risultato della query

    // Converti il JSON in una stringa (il chiamante deve liberare la memoria con `free`)
    char *json_string_result = json_dumps(loan_array, JSON_INDENT(4));
    json_decref(loan_array); // Libera la struttura JSON (ma non la stringa)

    return json_string_result;
}

char *select_all_expired_loans()
{

    PGresult *res = PQexecPrepared(conn, "select_all_expired_loans", 0, NULL, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "Errore nell'esecuzione della SELECT: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return NULL;
    }

    int rows = PQntuples(res);         // Numero di righe restituite
    json_t *loan_array = json_array(); // Array JSON per contenere i film

    for (int i = 0; i < rows; i++)
    {
        json_t *loan_json = json_object(); // Oggetto JSON per un singolo film

        json_object_set_new(loan_json, "id", json_string(PQgetvalue(res, i, 0)));
        json_object_set_new(loan_json, "film_id", json_string(PQgetvalue(res, i, 1)));
        json_object_set_new(loan_json, "user_id", json_string(PQgetvalue(res, i, 2)));
        json_object_set_new(loan_json, "checkout_date", json_string(PQgetvalue(res, i, 3)));
        json_object_set_new(loan_json, "due_date", json_string(PQgetvalue(res, i, 4)));
        json_object_set_new(loan_json, "return_date", json_string(PQgetvalue(res, i, 5)));
        json_object_set_new(loan_json, "film_title", json_string(PQgetvalue(res, i, 6)));
        json_object_set_new(loan_json, "username", json_string(PQgetvalue(res, i, 7)));

        json_array_append_new(loan_array, loan_json); // Aggiunge il film all'array JSON
    }

    PQclear(res); // Libera il risultato della query

    // Converti il JSON in una stringa (il chiamante deve liberare la memoria con `free`)
    char *json_string_result = json_dumps(loan_array, JSON_INDENT(4));
    json_decref(loan_array); // Libera la struttura JSON (ma non la stringa)

    return json_string_result;
}

bool select_film_id_by_title(char *title, char *film_id)
{
    const char *paramValues[1] = {title};

    // Esegui la query preparata per ottenere l'ID utente in base allo username
    PGresult *res = PQexecPrepared(conn, "select_film_id_by_title", 1, paramValues, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        // Se c'è un errore, stampiamo un messaggio di errore e liberiamo la memoria
        fprintf(stderr, "Errore nella SELECT per title: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return false;
    }

    if (PQntuples(res) == 0)
    {
        // Se non ci sono righe nel risultato, l'utente non è stato trovato
        printf("film non trovato.\n");
        PQclear(res);
        return false;
    }
    else
    {
        strcpy(film_id, PQgetvalue(res, 0, 0));
        printf("ID film: %s\n", film_id);
    }

    PQclear(res);
    return true;
}

void database_close_connection()
{
    if (conn != NULL)
    {
        PQfinish(conn); // Chiude la connessione al database
        conn = NULL;    // Previene dangling pointers
    }
}

/* FUNZIONI AUSILIARIE */
bool can_user_loan(char *user_id)
{
    PGresult *res;

    // Query to get the active loans count and max_loans for the user in one step
    const char *check_loans_query =
        "SELECT COUNT(*) AS active_loans, max_loans "
        "FROM loans JOIN users ON loans.user_id = users.id "
        "WHERE loans.user_id = $1 AND loans.return_date IS NULL "
        "GROUP BY users.id";

    // Prepare parameter values for the query
    const char *paramValuesLoans[1] = {user_id};

    // Execute the query to check the active loans and max_loans
    res = PQexecParams(conn, check_loans_query, 1, NULL, paramValuesLoans, NULL, NULL, 0);
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "Error executing check_loans_query: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return false;
    }

    // Check if any rows were returned
    int numRows = PQntuples(res);
    if (numRows == 0)
    {
        // If no rows are returned, the user has no active loans
        PQclear(res);
        return true;
    }

    // Get active_loans and max_loans from the result
    int active_loans = atoi(PQgetvalue(res, 0, 0)); // Get active loans count
    int max_loans = atoi(PQgetvalue(res, 0, 1));    // Get max_loans for the user
    PQclear(res);                                   // Clear the result from the previous query

    // Check if the user has exceeded the maximum allowed loans
    if (active_loans >= max_loans)
    {
        return false;
    }

    return true;
}
