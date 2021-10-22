/*UDP Echo Client*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFF_SIZE 1024

int main(int argc, char **argv) {
    if(argc < 3) {
        printf("Usage: ./client IPAddress PortNumber\n");
        return 1;
    }
    int client_sock;
    char buff[BUFF_SIZE + 1] = "";
    struct sockaddr_in server_addr;
    int bytes_sent, bytes_received;
    socklen_t sin_size;
    
    //Step 1: Construct a UDP socket
    if ((client_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        /* calls socket() */
        perror("\nError: ");
        exit(2);
    }

    //Step 2: Define the address of the server
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    while(1) {
        //Step 3: Communicate with server
        printf("\nInsert string to send (Ctrl + C to quit): ");
        memset(buff, '\0', (strlen(buff) + 1));
        scanf("%[^\n]s", buff);
        while(getchar() != '\n');
        
        sin_size = (socklen_t) sizeof(struct sockaddr);
        
        bytes_sent = sendto(client_sock, buff, strlen(buff), 0, (struct sockaddr *) &server_addr, sin_size);
        if (bytes_sent < 0) {
            perror("Error: ");
            close(client_sock);
            return 2;
        }

        bytes_received = recvfrom(client_sock, buff, BUFF_SIZE - 1, 0, (struct sockaddr *) &server_addr, &sin_size);
        if (bytes_received < 0) {
            perror("Error: ");
            close(client_sock);
            return 2;
        }
        buff[bytes_received] = '\0';
        printf("\nReply from server:\n%s\n", buff);
    }
        
    close(client_sock);
    return 0;
}
