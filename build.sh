#!/bin/bash


# Dependencies : gcc

echo "Compiling the server.c"
gcc -o build/server server.c
echo "Compiling the client.c"
gcc -o build/client client.c

