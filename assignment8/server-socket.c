#include <ctype.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 5500
#define THREAD_MAX 3
#define MAX_SIZE 256

//Remember to use -pthread when compiling this server's source code
void *connection_handler(void *);

int main() {
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket == -1) {
        perror("Socket initialisation failed");
        exit(EXIT_FAILURE);
    } else
        printf("Server socket created successfully\n");

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    //bind the socket to the specified IP addr and port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    } else
        printf("Socket successfully binded..\n");

    if (listen(server_socket, 3) != 0) {
        printf("Listen failed...\n");
        exit(0);
    } else
        printf("Server listening..\n");

    int no_threads = 0;
    pthread_t threads[THREAD_MAX];
    while (no_threads < THREAD_MAX) {
        printf("Listening...\n");
        int client_socket = accept(server_socket, NULL, NULL);
        puts("Connection accepted");
        if (pthread_create(&threads[no_threads], NULL, connection_handler, &client_socket) < 0) {
            perror("Could not create thread");
            return 1;
        }
        if (client_socket < 0) {
            printf("server acccept failed...\n");
            exit(0);
        } else
            printf("Server acccept the client...\n");
        puts("Handler assigned");
        no_threads++;
    }
    int k = 0;
    for (k = 0; k < THREAD_MAX; k++) {
        pthread_join(threads[k], NULL);
    }
    close(server_socket);
    return 0;
}

void *connection_handler(void *client_socket) {
    int socket = *(int *)client_socket;
    int read_len;
    char server_message[100] = "Hello from server\n";
    send(socket, server_message, strlen(server_message), 0);
    char client_message[MAX_SIZE];
    while ((read_len = recv(socket, client_message, MAX_SIZE, 0)) > 0) {
        //end of string marker
        client_message[read_len] = '\0';
        if ((strcmp(client_message, "q") == 0) || (strcmp(client_message, "Q") == 0)) {
            break;
        }
        //Send the message back to client
        int len = strlen(client_message);
        for (int i = 0; i < len; i++) {
            if (!isalpha(client_message[i])) {
                strcpy(client_message, "Wrong text format");
                break;
            } else {
                client_message[i] = toupper(client_message[i]);
            }
        }
        send(socket, client_message, strlen(client_message), 0);
    }
    return 0;
}
