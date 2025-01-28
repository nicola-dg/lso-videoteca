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

struct client
{
    int socket;
    struct sockaddr address;
    socklen_t addrlen;
};

typedef struct client client_t;

int createSocket();
void bindSocket(int *sockfd);
void startListening(int *sockfd);
client_t *acceptConnection(int *sockfd);
void setupServerAddress(int PORT);
void startServer(int PORT);