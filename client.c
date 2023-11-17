#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define TRUE 1
#define BUFF_SIZE 255

void error(char *);
void clr(char *, int);

int main(int argc, char *argv[]) {
    if(argc < 3) {
        printf("ERROR: Some arguments missing!\n");
        printf("ERROR: usage: %s <inet-address> <port_no>\n",argv[0]);
        exit(1);
    }
    int port_number = atoi(argv[2]);



    /*
     * socket()
     *
     * connect()
     *
     * read() write()
     *
    */


    // The socket() to get the file descriptor  ****************************************
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd < 0) {
        error("Failed to create the socket\n");
    }
    else {
        printf("INFO: Created socket\n");
    }
    //printf("DEBUG: the fd : %d\n",fd);
    


    // The connect() to connect to the server socket  ***************************************
    struct sockaddr_in server_address;
    struct hostent *server;             // structure to store the inet address of the server
    server = gethostbyname(argv[1]);    // convert the inet to network byte order
    if(server == NULL) {
        error("No such host, change the inet-address\n");
    }
    else {
        printf("INFO: Converted inet-address to network byte order by gethostbyname()\n");
    }
    clr((char *) &server_address, sizeof(server_address));
    // Need to fill the server address values
    server_address.sin_family = AF_INET;
    bcopy((char *) server->h_addr, (char *) &server_address.sin_addr.s_addr, server->h_length);
    server_address.sin_port = htons(port_number);

    //printf("DEBUG: server_address.sin_family: %d\n", server_address.sin_family);
    //printf("DEBUG: server_address.sin_addr.s_addr: %d\n", server_address.sin_addr.s_addr);
    //printf("DEBUG: server_address.sin_port: %d\n", server_address.sin_port);
    //printf("DEBUG: server->h_addr: %d\n", server->h_addr);
    //printf("DEBUG: server->h_length: %d\n", server->h_length);

    // connect() to connect to the server  *******************************************
    int status = connect(fd, (struct sockaddr *) &server_address, sizeof(server_address));
    if(status < 0) {
        error("cannot connect to the server\n");
    }
    else {
        printf("INFO: Established connection to the server\n");
    }



    // Read the welcome message from the server
    char buffer[BUFF_SIZE];
    int bytes = 0;
    // read welcome
    clr(buffer, BUFF_SIZE);
    bytes = read(fd, buffer, BUFF_SIZE);
    if(bytes < 0) {
        error("cannot read the welcome message from the server\n");
    }
    else {
        printf("Server: ");
    }
    printf("%s", buffer);

    // infinite loop to read ans write
    while(TRUE) {
        // write
        clr(buffer, BUFF_SIZE);
        printf("You: ");
        fgets(buffer, BUFF_SIZE, stdin);
        bytes = write(fd, buffer, strlen(buffer));
        if(bytes < 0) {
            error("cannot send message to the server\n");
        }
        else {
            printf("INFO: message sent\n");
        }
        
        // read
        clr(buffer, BUFF_SIZE);
        bytes = read(fd, buffer, BUFF_SIZE);
        if(bytes < 0) {
            error("cannot read from the server\n");
        }
        else {
            printf("Server: ");
        }
        printf("%s", buffer);
    }

    



    return 0;
}

void error(char *err) {
    printf("ERROR: %s\n",err);
    exit(1);
}

void clr(char *ptr, int len) {
    bzero(ptr, len);
}
