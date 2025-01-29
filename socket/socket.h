#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "socket.h"
#include <arpa/inet.h>
#include "../json_parsing/json.h"
#include "../json_parsing/request_print.h"

struct client
{
    int socket;
    struct sockaddr address;
    socklen_t addrlen;
};

typedef struct client client_t;

int create_socket();
void bind_socket(int *sockfd);
void start_listening(int *sockfd);
client_t *accept_connection(int *sockfd);
void setup_server_address(int PORT);
void start_server(int PORT);