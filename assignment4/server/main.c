/*UDP Echo Server*/
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#define BUFF_SIZE 1024

char *handleString(char *string) {
    if (strlen(string) == 0) {
        return NULL;
    }
    int countNumber = 0, countLetter = 0;
    char *res = malloc(BUFF_SIZE + 1);
    char number[BUFF_SIZE], letter[BUFF_SIZE];
    for (int i = 0; string[i] != '\0'; i++) {
        if (isdigit(string[i])) {
            number[countNumber++] = string[i];
        } else if (isalpha(string[i])) {
            letter[countLetter++] = string[i];
        } else {
            return NULL;
        }
    }
    number[countNumber] = '\0';
    letter[countLetter] = '\0';
    sprintf(res, "%s\n%s", number, letter);
    return res;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Not found port number\n");
        return 1;
    }

    int server_sock; /* file descriptors */
    char buff[BUFF_SIZE];
    int bytes_sent, bytes_received;
    struct sockaddr_in server; /* server's address information */
    struct sockaddr_in client; /* client's address information */
    socklen_t sin_size;

    //Step 1: Construct a UDP socket
    if ((server_sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        /* calls socket() */
        perror("\nError: ");
        exit(2);
    }

    //Step 2: Bind address to socket
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[1])); /* Remember htons() from "Conversions" section? =) */
    server.sin_addr.s_addr = INADDR_ANY; /* INADDR_ANY puts your IP address automatically */
    bzero(&(server.sin_zero), 8); /* zero the rest of the structure */

    if (bind(server_sock, (struct sockaddr *)&server, (socklen_t) sizeof(struct sockaddr)) == -1) {
        /* calls bind() */
        perror("\nError: ");
        exit(2);
    }

    //Step 3: Communicate with clients
    while (1) {
        sin_size = sizeof(struct sockaddr_in);

        bytes_received = recvfrom(server_sock, buff, BUFF_SIZE - 1, 0, (struct sockaddr *)&client, &sin_size);

        if (bytes_received < 0)
            perror("\nError: ");
        else {
            // Handle string
            buff[bytes_received] = '\0';
            char *res = handleString(buff);
            if (res == NULL) {
                strcpy(res, "Error");
            }
            bytes_sent = sendto(server_sock, res, strlen(res), 0, (struct sockaddr *) &client, sin_size);
            if (bytes_sent < 0) {
                perror("\nError: ");
            }
        }

        bytes_sent = sendto(server_sock, buff, bytes_received, 0, (struct sockaddr *)&client, sin_size); /* send to the client welcome message */
        if (bytes_sent < 0) {
            perror("\nError: ");
        }
    }

    close(server_sock);
    return 0;
}
