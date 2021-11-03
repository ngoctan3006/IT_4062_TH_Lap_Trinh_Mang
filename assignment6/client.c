#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MAX_SIZE 1024


int main(int argc, char **argv) {
    if (argc != 4) {
        printf("Usage: ./client IPAddress PortNumber Path\n");
        return -1;
    }
    int client_sock;
    struct sockaddr_in server_addr; /* server's address information */
    int bytes_sent, bytes_received;
    char buff[MAX_SIZE + 1];

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
    //check file is exists
    FILE *fp = fopen(argv[3], "rb");
    char sentBuff[MAX_SIZE + 1];
    if(!fp) {
        strcpy(sentBuff, "not found");
    } else {
        strcpy(sentBuff, argv[3]);
    }
    //send filename
    bytes_sent = send(client_sock, sentBuff, strlen(sentBuff), 0);
    if (bytes_sent <= 0) {
        printf("\nConnection closed!\n");
        return 2;
    }

    //receive status
    char status[MAX_SIZE];
    bytes_received = recv(client_sock, status, MAX_SIZE - 1, 0);
    if (bytes_received <= 0) {
        printf("\nError! Cannot receive data from sever!\n");
        return 2;
    }
    status[bytes_received] = '\0';
    if(strcmp(status, "exist") == 0) {
        printf("Error: File already exists\n");
        return 1;
    } else if(strcmp(status, "error") == 0) {
        printf("Error: Wrong file format\n");
        return 1;
    } else if (strcmp(status, "not found") == 0) {
        printf("Error: File not found\n");
        return 1;
    }

    printf("\nSending...\n");
    int n;
    while ((n = fread(buff, 1, MAX_SIZE, fp)) > 0) {
        send(client_sock, buff, n, 0);
    }
    printf("\nSuccess: File transfer is completed\n");
    fclose(fp);

    //Step 4: Close socket
    close(client_sock);
    return 0;
}
