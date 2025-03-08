#pragma once

#include <libpq-fe.h>
#include <stdbool.h>
#include <jansson.h>
#include <string.h>

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
void prepare_select_statements();

// Funzioni per l'inserimento dei dati
bool insert_user(const char *username, const char *password, const char *email, const char *name, const char *surname, const char *role);
bool insert_film(const char *title, const char *genre, const char *price);
bool insert_loan(char *film_id, char *user_id);
bool insert_cart(char *movie_id, char *user_id);

// Funzioni per l'aggiornamento dei dati
bool update_user(const char *password, const char *email, const char *name, const char *surname, int max_loans, const char *username);
bool update_film(int available_copies, int movie_id);
bool update_loan(const char *return_date, int loan_id);
bool update_cart(const char *checkout_date, int cart_id);
bool update_film_return(char *film_id, char *user_id);

// Funzioni per l'eliminazione dei dati
bool delete_user(int user_id);
bool delete_film(int movie_id);
bool delete_loan(int loan_id);
bool delete_cart(int cart_id);

// Funzione per leggere i dati
void select_all_users();
void select_user_by_id(int user_id);
bool select_user_by_username_and_password(const char *username, const char *password);
bool select_user_id_by_username(char *username, char *user_id);
bool select_film_id_by_title(char *title, char *film_id);

char *select_all_films();
char *select_active_loans_by_id(char *user_id);
char *select_all_expired_loans();

// Funzione di esecuzione di query preparate
bool execute_prepared_statement(const char *stmt_name, int nParams, const char **paramValues);

void database_start(bool reset);

// ausiliarie
bool can_user_loan(char *user_id);