#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 5500
#define MAX_SIZE 256

int main() {
    //create a socket
    int network_socket;
    network_socket = socket(AF_INET, SOCK_STREAM, 0);

    //specify an address for the socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    int connection_status = connect(network_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    //check for connection_status
    if (connection_status == -1) {
        printf("The connection has error\n\n");
    }

    if (connection_status == 0) {
        int total_bytes = 0, recv_bytes;
        char response[MAX_SIZE];
        char request[MAX_SIZE];
        //receive data from the server
        recv(network_socket, &response, MAX_SIZE, 0);
        memset(response, '\0', MAX_SIZE);
        while (1) {
            //char response[256];
            printf("enter a message to echo\n");
            scanf("%[^\n]", request);
            while (getchar() != '\n');
            send(network_socket, request, strlen(request), 0);
            total_bytes += strlen(request);
            if ((strcmp(request, "q") == 0) || (strcmp(request, "Q") == 0)) {
                printf("Total bytes sent to server: %d bytes\n", total_bytes);
                break;
            }
            //print out the server's response
            recv_bytes = recv(network_socket, &response, MAX_SIZE, 0);
            response[recv_bytes] = '\0';
            printf("Here is the echo message from the server: %s\n\n", response);
        }
    }
    //close the socket
    close(network_socket);

    return 0;
}
