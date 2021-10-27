#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#define BUFF_SIZE 1024
#define MAX_SIZE 256

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Usage: ./client IPAddress PortNumber\n");
        return -1;
    }
    int client_sock;
    char buff[BUFF_SIZE + 1], username[MAX_SIZE], password[MAX_SIZE];
    struct sockaddr_in server_addr; /* server's address information */
    int msg_len, bytes_sent, bytes_received;

    //Step 1: Construct socket
    if ((client_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("\nError ");
    }

    //Step 2: Specify server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    //Step 3: Request to connect server
    if (connect(client_sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) < 0) {
        printf("Error! Can not connect to sever! Client exit imediately!\n");
        return 0;
    }

    //Step 4: Communicate with server
    while (1) {
        //send message
        memset(buff, '\0', (strlen(buff) + 1));
        memset(username, '\0', (strlen(username) + 1));
        memset(password, '\0', (strlen(password) + 1));
        printf("Enter username: ");
        scanf("%s", username);
        while(getchar() != '\n');
        printf("Enter password: ");
        scanf("%s", password);
        while(getchar() != '\n');
        sprintf(buff, "%s\n%s", username, password);
        msg_len = strlen(buff);
        if (msg_len == 0) break;

        bytes_sent = send(client_sock, buff, msg_len, 0);
        if (bytes_sent <= 0) {
            printf("\nConnection closed!\n");
            break;
        }

        //receive echo reply
        char recv_data[MAX_SIZE];
        int isClose = 0;
        bytes_received = recv(client_sock, recv_data, MAX_SIZE - 1, 0);
        if (bytes_received <= 0) {
            printf("\nError! Cannot receive data from sever!\n");
            break;
        }
        recv_data[bytes_received] = '\0';
        switch (atoi(recv_data)) {
            case 0:
                printf("  -> Account is blocked or inactive!\n\n");
                break;
            case 1:
                printf("  -> Password is not correct. Please try again!\n\n");
                break;
            case 2:
                printf("  -> Account is blocked!\n\n");
                break;
            case 3:
                printf("  -> Login is successful!\n\n");
                do {
                    printf("  Enter \"Bye\" to logout: ");
                    scanf("%s", buff);
                    while(getchar() != '\n');
                } while (strcmp(buff, "Bye") != 0);
                msg_len = strlen(buff);
                bytes_sent = send(client_sock, buff, msg_len, 0);
                if(bytes_sent <= 0) {
                    isClose = 1;
                    break;
                }
                bytes_received = recv(client_sock, recv_data, MAX_SIZE - 1, 0);
                if (bytes_received <= 0) {
                    isClose = 2;
                    break;
                }
                recv_data[bytes_received] = '\0';
                printf("  -> %s\n\n", recv_data);
                break;
        }
        if (isClose == 1) {
            printf("\nConnection closed!\n");
            break;
        } else if (isClose == 2) {
            printf("\nError! Cannot receive data from sever!\n");
            break;
        }
    }

    //Step 4: Close socket
    close(client_sock);
    return 0;
}
