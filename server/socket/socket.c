
#include "socket.h"
#define BUFFERSIZE 8192

struct sockaddr_in serv_addr, client_addr;
socklen_t client_len = sizeof(client_addr);

void *handle_client(void *client_void_ptr)
{
    client_t *client = (client_t *)client_void_ptr;

    char buffer[BUFFERSIZE] = {0};
    int bytesRead;

    database_start(true);
    /*-------------------------------------------------*/
    /*----------LOGICA CONNESSIONE DATABASE------------*/
    /*-------------------------------------------------*/

    // Ciclo per continuare a leggere finché il client non invia "Connection: close"
    while (1)
    {
        // Leggi i dati dal client
        bytesRead = read(client->socket, buffer, BUFFERSIZE - 1); // Leggi i dati, lasciando spazio per '\0'
        if (bytesRead == -1)
        {
            perror("Error reading from client");
            break; // Esci dal ciclo in caso di errore
        }
        else if (bytesRead == 0)
        {
            // Il client ha chiuso la connessione in modo ordinato
            break;
        }

        // Null-terminate the buffer
        buffer[bytesRead] = '\0';

        // Visualizza i dati ricevuti
        // printf("Received from client-%d:\n%s\n", client->socket, buffer);

        /*-------------------------------------------------*/
        /*-----------LOGICA PARSING RICHIESTA--------------*/
        /*-------------------------------------------------*/
        request_t *req = decode(buffer);
        route_request(req, client->socket);

        // Se il client invia "Connection: close", interrompi la comunicazione
        // if (isConnectionClosed())
        //{
        /*-------------------------------------------------*/
        /*----------LOGICA CHIUSURA CONNESSIONE------------*/
        /*-------------------------------------------------*/
        //}
    }

    printf("closing connection with client-%d\n", client->socket);
    close(client->socket); // Chiudi la connessione al client
    free(client);
    return NULL;
}

int create_socket()
{
    int sockfd;
    int opt = 1;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Configure socket options
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        perror("Error configuring socket options");
        exit(EXIT_FAILURE);
    }

    return sockfd;
}

void bind_socket(int *sockfd)
{
    if (bind(*sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        perror("Error binding socket address");
        exit(EXIT_FAILURE);
    }
}

void start_listening(int *sockfd)
{
    if (listen(*sockfd, 3) == -1)
    {
        perror("Error listening on socket");
        exit(EXIT_FAILURE);
    }
}

client_t *accept_connection(int *sockfd)
{
    client_t *client = (client_t *)malloc(sizeof(client_t));

    client->addrlen = sizeof(client->address);
    client->socket = accept(*sockfd, &client->address, &client->addrlen);
    return client;
}

void setup_server_address(int port)
{
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
}

void start_server(int port)
{
    int serverSocket = create_socket();
    setup_server_address(port);

    bind_socket(&serverSocket);
    start_listening(&serverSocket);

    printf("Server running on port %d...\n", port);

    while (1)
    {
        // Accetta la connessione del client
        client_t *client_ptr = accept_connection(&serverSocket);

        // Crea un nuovo thread per gestire la connessione del client
        pthread_t thread;

        if (pthread_create(&thread, NULL, handle_client, client_ptr) != 0)
        {
            perror("Error creating thread");
            free(client_ptr);
        }
        else
        {
            printf("Thread created with ID %ld\n", thread);
            pthread_detach(thread); // Stacchiamo il thread, così non dobbiamo aspettarlo
        }
    }

    close(serverSocket); // Chiudiamo il socket del server
}
