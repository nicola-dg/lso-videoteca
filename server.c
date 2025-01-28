#include <stdlib.h>
#include <stdio.h>
#include <jansson.h>
#include <string.h>

#include "socket/socket.h"

int main()
{
    printf("Starting server...\n");
    startServer(8080);
    return 0;
}