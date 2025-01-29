#!/bin/bash

gcc -o main server.c socket/socket.c json_parsing/json_decode.c json_parsing/request_print.c -ljansson