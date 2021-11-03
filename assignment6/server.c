#include <stdio.h> /* These are the usual header files */
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>

#define BACKLOG 5 /* Number of allowed connections */
#define MAX_SIZE 1024

int checkFileFormat(char *path);
char *getFilename(char *path);

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: ./server PortNumber\n");
        return -1;
    }
    int listen_sock, conn_sock; /* file descriptors */
    char recv_data[MAX_SIZE + 1];
    int bytes_sent, bytes_received;
    struct sockaddr_in server; /* server's address information */
    struct sockaddr_in client; /* client's address information */
    socklen_t sin_size;

    // Step 1: Construct a TCP socket to listen connection request
    if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        /* calls socket() */
        perror("\nError ");
        return 0;
    }

    // Step 2: Bind address to socket
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[1])); /* Remember htons() from "Conversions" section? =) */
    server.sin_addr.s_addr = htonl(INADDR_ANY); /* INADDR_ANY puts your IP address automatically */
    if (bind(listen_sock, (struct sockaddr *)&server, sizeof(server)) == -1) {
        /* calls bind() */
        perror("\nError ");
        return 0;
    }

    // Step 3: Listen request from client
    if (listen(listen_sock, BACKLOG) == -1) {
        /* calls listen() */
        perror("\nError ");
        return 0;
    }

    printf("Received files will be saved to \"/recv\" folder\n\n");
    // Step 4: Communicate with client
    while (1) {
        // accept request
        sin_size = sizeof(struct sockaddr_in);
        if ((conn_sock = accept(listen_sock, (struct sockaddr *)&client, &sin_size)) == -1)
            perror("\nError ");

        printf("You got a connection from %s\n", inet_ntoa(client.sin_addr)); /* prints client's IP */
        bytes_received = recv(conn_sock, recv_data, MAX_SIZE, 0);
        if (bytes_received <= 0) {
            printf("\nConnection closed\n");
            break;
        }
        mkdir("recv", S_IRWXU);
        recv_data[bytes_received] = '\0';
        char filename[MAX_SIZE] = "recv/";
        strcat(filename, getFilename(recv_data));
        FILE *fp = fopen(filename, "r");
        char reply[MAX_SIZE + 1];
        int isError = 0;
        if(strcmp(recv_data, "not found") == 0) {
            strcpy(reply, recv_data);
            isError = 1;
        }else if(!checkFileFormat(recv_data)) {
            strcpy(reply, "error");
            isError = 1;
        }
        else if(!fp) {
            strcpy(reply, "ok");
        } else {
            strcpy(reply, "exist");
            isError = 1;
        }
        bytes_sent = send(conn_sock, reply, strlen(reply), 0);
        if (bytes_sent <= 0) {
            printf("\nConnection closed\n");
            break;
        }
        if(!isError) {
            fp = fopen(filename, "wb");
            if(!fp) {
                printf("Cannot open file!\n");
                return 1;
            }
            int n;
            while ((n = recv(conn_sock, recv_data, MAX_SIZE, 0)) > 0) {
                fwrite(recv_data, sizeof(char), n, fp);
            }
            fclose(fp);
        }
        close(conn_sock);
    }

    close(listen_sock);
    return 0;
}

int checkFileFormat(char *path) {
    int len = strlen(path);
    char temp[MAX_SIZE];
    while(len > 0) {
        if(path[len - 1] != '.')
            len--;
        else break;
    }
    strcpy(temp, &path[len]);
    if(strcmp(temp, "jpg") == 0 || strcmp(temp, "bmp") == 0 || strcmp(temp, "png") == 0)
        return 1;
    return 0;
}

char *getFilename(char *path) {
    int len = strlen(path);
    char *temp = (char *) malloc(MAX_SIZE + 1);
    while(len > 0) {
        if(path[len - 1] != '/')
            len--;
        else break;
    }
    strcpy(temp, &path[len]);
    return temp;
}
