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
        "TRUNCATE TABLE movies RESTART IDENTITY CASCADE;",
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
        "max_loans INTEGER DEFAULT 3);",

        "CREATE TABLE IF NOT EXISTS movies ("
        "id SERIAL PRIMARY KEY, "
        "title TEXT NOT NULL, "
        "genre TEXT, "
        "total_copies INTEGER, "
        "available_copies INTEGER, "
        "loan_count INTEGER DEFAULT 1);",

        "CREATE TABLE IF NOT EXISTS loans ("
        "id SERIAL PRIMARY KEY, "
        "movie_id INTEGER REFERENCES movies(id) ON DELETE CASCADE, "
        "user_id INTEGER REFERENCES users(id) ON DELETE CASCADE, "
        "checkout_date TIMESTAMP DEFAULT NOW(),"
        "due_date TIMESTAMP, "
        "return_date TIMESTAMP);",

        "CREATE TABLE IF NOT EXISTS carts ("
        "id SERIAL PRIMARY KEY, "
        "movie_id INTEGER REFERENCES movies(id) ON DELETE CASCADE, "
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

void execute_prepared_statement(const char *stmt_name, int nParams, const char **paramValues)
{
    // Esegui la query preparata con i parametri passati
    PGresult *res = PQexecPrepared(conn, stmt_name, nParams, paramValues, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK && PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        // Se la query non va a buon fine, stampa l'errore
        fprintf(stderr, "Errore nell'esecuzione della query '%s': %s\n", stmt_name, PQerrorMessage(conn));
    }
    else
    {
        // Se la query è stata eseguita correttamente
        printf("Esecuzione riuscita della query '%s'.\n", stmt_name);
    }

    // Pulisci il risultato della query
    PQclear(res);
}

void prepare_insert_statements()
{
    PGresult *res;

    // Statement per inserire un utente
    res = PQprepare(conn, "insert_user",
                    "INSERT INTO users (username, password, email, name, surname) VALUES ($1, $2, $3, $4, $5);",
                    5, NULL);
    PQclear(res);

    // Statement per inserire un film
    res = PQprepare(conn, "insert_movie",
                    "INSERT INTO movies (title, genre, total_copies, available_copies, loan_count) VALUES ($1, $2, $3, $4, $5);",
                    5, NULL);
    PQclear(res);

    // Statement per inserire un prestito
    res = PQprepare(conn, "insert_loan",
                    "INSERT INTO loans (movie_id, user_id, due_date) VALUES ($1, $2, $3);",
                    3, NULL);
    PQclear(res);

    // Statement per inserire un carrello
    res = PQprepare(conn, "insert_cart",
                    "INSERT INTO carts (movie_id, user_id, checkout_date) VALUES ($1, $2, $3);",
                    3, NULL);
    PQclear(res);
}

void insert_user(const char *username, const char *password, const char *email, const char *name, const char *surname)
{
    const char *paramValues[5] = {username, password, email, name, surname};
    execute_prepared_statement("insert_user", 5, paramValues);
}

void insert_movie(const char *title, const char *genre, int total_copies, int available_copies, int loan_count)
{
    const char *paramValues[5] = {title, genre, (const char *)&total_copies, (const char *)&available_copies, (const char *)&loan_count};
    execute_prepared_statement("insert_movie", 5, paramValues);
}

void insert_loan(int movie_id, int user_id, const char *due_date)
{
    const char *paramValues[3] = {(const char *)&movie_id, (const char *)&user_id, due_date};
    execute_prepared_statement("insert_loan", 3, paramValues);
}

void insert_cart(int movie_id, int user_id, const char *checkout_date)
{
    const char *paramValues[3] = {(const char *)&movie_id, (const char *)&user_id, checkout_date};
    execute_prepared_statement("insert_cart", 3, paramValues);
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
    res = PQprepare(conn, "update_movie",
                    "UPDATE movies SET available_copies = $1 WHERE id = $2;",
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
}

void update_user(const char *password, const char *email, const char *name, const char *surname, int max_loans, const char *username)
{
    const char *paramValues[6] = {password, email, name, surname, (const char *)&max_loans, username};
    execute_prepared_statement("update_user", 6, paramValues);
}

void update_movie(int available_copies, int movie_id)
{
    const char *paramValues[2] = {(const char *)&available_copies, (const char *)&movie_id};
    execute_prepared_statement("update_movie", 2, paramValues);
}

void update_loan(const char *return_date, int loan_id)
{
    const char *paramValues[2] = {return_date, (const char *)&loan_id};
    execute_prepared_statement("update_loan", 2, paramValues);
}

void update_cart(const char *checkout_date, int cart_id)
{
    const char *paramValues[2] = {checkout_date, (const char *)&cart_id};
    execute_prepared_statement("update_cart", 2, paramValues);
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
    res = PQprepare(conn, "delete_movie",
                    "DELETE FROM movies WHERE id = $1;",
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

void delete_user(int user_id)
{
    const char *paramValues[1] = {(const char *)&user_id};
    execute_prepared_statement("delete_user", 1, paramValues);
}

void delete_movie(int movie_id)
{
    const char *paramValues[1] = {(const char *)&movie_id};
    execute_prepared_statement("delete_movie", 1, paramValues);
}

void delete_loan(int loan_id)
{
    const char *paramValues[1] = {(const char *)&loan_id};
    execute_prepared_statement("delete_loan", 1, paramValues);
}

void delete_cart(int cart_id)
{
    const char *paramValues[1] = {(const char *)&cart_id};
    execute_prepared_statement("delete_cart", 1, paramValues);
}

void read_data()
{
    PGresult *res = PQexec(conn, "SELECT id, username, password FROM users;");

    if (PQresultStatus(res) == PGRES_TUPLES_OK)
    {
        int rows = PQntuples(res);
        for (int i = 0; i < rows; i++)
        {
            printf("ID: %s, username: %s, password: %s\n",
                   PQgetvalue(res, i, 0),
                   PQgetvalue(res, i, 1),
                   PQgetvalue(res, i, 2));
        }
    }
    else
    {
        fprintf(stderr, "Errore nella query: %s\n", PQerrorMessage(conn));
    }
    PQclear(res);
}

void database_close_connection()
{
    if (conn != NULL)
    {
        PQfinish(conn); // Chiude la connessione al database
        conn = NULL;    // Previene dangling pointers
    }
}