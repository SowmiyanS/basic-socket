#include <stdio.h> // For printf
#include <string.h> // For bzero
#include <stdlib.h> // For exit

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define TRUE 1        // 1 means infinite loop
#define BUFF_SIZE 255 // buffer size

void error(char *);
void clr(char *, int);

int main(int argc, char *argv[]) {
    if(argc < 2) {
        printf("ERROR: usage: %s port\n", argv[0]);
    }
    int port_number = atoi(argv[1]);


    
    /*
     * socket()
     *
     * bind()
     *
     * listen()
     *
     * accept()
     *
     * read() and write()
     *
    */



    // The socket() to get file descriptor  ******************************************
    int fd = socket(AF_INET, SOCK_STREAM, 0); // get the file descriptor
    if(fd < 0) {
        error("Failed to create the socket\n");
    }
    else {
        printf("INFO: Created socket\n");
    }
    //printf("DEBUG: the fd : %d\n",fd);
    


    // The bind() to bind to a ip address and a port number  ******************************************
    struct sockaddr_in server_address;
    // Need to fill the address values!
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port_number);
    
    //printf("DEBUG: family : %d\n",server_address.sin_family);
    //printf("DEBUG: address : %d\n",server_address.sin_addr.s_addr);
    //printf("DEBUG: port : %d\n",server_address.sin_port);

    // Initialized the server address structure with the necessary values now we can pass it to the bind() to get an ip and port from the operating system.
    int status = bind(fd, (struct sockaddr *) &server_address, sizeof(server_address));
    if(status < 0) {
        printf("ERROR: Failed to bind to the port %s\n",argv[1]);
        exit(1);
    }
    else {
        printf("INFO: Bind the port: %s\n",argv[1]);
    }



    // The Listen() to listen for the incoming connections  ******************************************
    status = listen(fd, 1); // set backlog to 1 and reusing file descriptor fd and status interger
    if(status < 0) {
        error("Listen failed\n");
    }
    else {
        printf("INFO: Listening on the given port: %s\n",argv[1]);
    }

    

    // The accept() is to accept the incoming connection [client] for future communications  ******************************************
    // now create the address for client
    struct sockaddr_in client_address;
    socklen_t client_len;
    client_len = sizeof(client_address);
    int nfd = accept(fd, (struct sockaddr *) &client_address, &client_len); // create a new socket file descriptor to hold the new connection and pass the socket file descriptor
                                                                            //
    //printf("DEBUG: the nfd : %d\n",nfd);

    if(nfd < 0) {
        error("Accept failed\n");
    }
    else {
        printf("INFO: Established [accepted] a new connection to a client\n");
    }

    // now we can read and write to the nfd to communicate to the client.
    // The read() and write() functions in a while loop ********************************************
    char buffer[BUFF_SIZE];
    int bytes = 0;

    // send welcome message
    clr(buffer, BUFF_SIZE);
    strcpy(buffer, "Welcome you are successfully connected to the server!\nYou can now send messages to the sever to get replay from the server\n");
    bytes = write(nfd, buffer, strlen(buffer));
    if(bytes < 0) {
        error("cannot send welcome message\n");
    }
    else {
        printf("INFO: sent the welcome message to the client\n");
    }
    
    // create an infinite loop
    while(TRUE) {
        clr(buffer, BUFF_SIZE); // clear the buffer
        bytes = read(nfd, buffer, BUFF_SIZE);
        if(bytes < 0) {
            error("cannot read from the client\n");
        }
        else {
            printf("INFO: Client: ");
        }
        printf("%s", buffer);

        // *******************************************
        clr(buffer, BUFF_SIZE);
        printf("You [server]: ");
        fgets(buffer, BUFF_SIZE, stdin);
        bytes = write(nfd, buffer, strlen(buffer));
        if(bytes < 0) {
            error("cannot write to the client\n");
        }
        else {
            printf("INFO: Message sent\n");
        }

    }


    return 0;
}

// Error function
void error(char *err) {
    printf("ERROR: %s\n",err);
    exit(1);
}

// Clear function using bzero()
void clr(char *ptr, int len) {
    bzero(ptr, len);
}
