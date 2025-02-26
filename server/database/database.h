#pragma once

#include <libpq-fe.h>
#include <stdbool.h>

// Funzioni per la connessione al database
void database_connect();
void database_close_connection();

// Funzione per creare le tabelle nel database
void create_tables();

// Funzione per resettare (svuotare) le tabelle
void reset_tables();

// Funzioni per le query preparate
void prepare_insert_statements();
void prepare_update_statements();
void prepare_delete_statements();

// Funzioni per l'inserimento dei dati
void insert_user(const char *username, const char *password, const char *email, const char *name, const char *surname, int max_loans);
void insert_movie(const char *title, const char *genre, int total_copies, int available_copies, int loan_count);
void insert_loan(int movie_id, int user_id, const char *due_date);
void insert_cart(int movie_id, int user_id, const char *checkout_date);

// Funzioni per l'aggiornamento dei dati
void update_user(const char *password, const char *email, const char *name, const char *surname, int max_loans, const char *username);
void update_movie(int available_copies, int movie_id);
void update_loan(const char *return_date, int loan_id);
void update_cart(const char *checkout_date, int cart_id);

// Funzioni per l'eliminazione dei dati
void delete_user(int user_id);
void delete_movie(int movie_id);
void delete_loan(int loan_id);
void delete_cart(int cart_id);

// Funzione per leggere i dati
void read_data();

// Funzione di esecuzione di query preparate
void execute_prepared_statement(const char *stmt_name, int nParams, const char **paramValues);

void database_start(bool reset);
