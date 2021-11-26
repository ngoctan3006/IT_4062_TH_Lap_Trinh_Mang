#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define BACKLOG 20
#define BUFF_SIZE 1024

/* Handler process signal*/
void sig_chld(int signo);

void fileHandler(int sockfd);
char *upperCase(char *str);

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: ./server PortNumber\n");
        return -1;
    }
    int listen_sock, conn_sock; /* file descriptors */
    struct sockaddr_in server;  /* server's address information */
    struct sockaddr_in client;  /* client's address information */
    pid_t pid;
    int sin_size;

    if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) { /* calls socket() */
        printf("socket() error\n");
        return 0;
    }

    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[1]));
    server.sin_addr.s_addr = htonl(INADDR_ANY); /* INADDR_ANY puts your IP address automatically */

    if (bind(listen_sock, (struct sockaddr *)&server, sizeof(server)) == -1) {
        perror("\nError: ");
        return 0;
    }

    if (listen(listen_sock, BACKLOG) == -1) {
        perror("\nError: ");
        return 0;
    }

    /* Establish a signal handler to catch SIGCHLD */
    signal(SIGCHLD, sig_chld);

    while (1) {
        sin_size = sizeof(struct sockaddr_in);
        if ((conn_sock = accept(listen_sock, (struct sockaddr *)&client, &sin_size)) == -1) {
            if (errno == EINTR)
                continue;
            else {
                perror("\nError: ");
                return 0;
            }
        }

        /* For each client, fork spawns a child, and the child handles the new client */
        pid = fork();

        /* fork() is called in child process */
        if (pid == 0) {
            close(listen_sock);
            printf("You got a connection from %s\n", inet_ntoa(client.sin_addr)); /* prints client's IP */
            fileHandler(conn_sock);
            exit(0);
        }

        /* The parent closes the connected socket since the child handles the new client */
        close(conn_sock);
    }
    close(listen_sock);
    return 0;
}

void sig_chld(int signo) {
    pid_t pid;
    int stat;

    /* Wait the child process terminate */
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
        printf("\nChild %d terminated\n", pid);
}

void fileHandler(int sockfd) {
    char buff[BUFF_SIZE + 1], fileName[BUFF_SIZE + 1];
    int bytes_sent, bytes_received, n;
    char c;

    bytes_received = recv(sockfd, buff, 10, 0);
    if (bytes_received < 0)
        perror("\nError: ");
    else if (bytes_received == 0)
        printf("Connection closed.");
    buff[bytes_received] = '\0';
    bytes_received = recv(sockfd, fileName, atoi(buff), 0);
    if (bytes_received < 0)
        perror("\nError: ");
    else if (bytes_received == 0)
        printf("Connection closed.");
    fileName[bytes_received] = '\0';
    upperCase(fileName);
    strcat(fileName, ".txt");
    FILE *fp = fopen(fileName, "wb");
    if (!fp) {
        printf("[!] Cannot open your file!");
        return;
    }
    char isDone[5];
    int total_bytes = 0;
    while ((n = recv(sockfd, buff, BUFF_SIZE, 0)) > 0) {
        // buff[n] = '\0';
        memcpy(isDone, &buff[n-4], 4);
        isDone[4] = '\0';
        if(strcmp(isDone, "done") == 0) {
            buff[n-4] = '\0';
            n -= 4;
        }
        fwrite(buff, sizeof(char), n, fp);
        total_bytes += n;
        if(strcmp(isDone, "done") == 0) break;
    }
    fclose(fp);
    fp = fopen(fileName, "r");
    FILE *f = fopen("res.txt", "w");
    if (!fp || !f) {
        printf("[!] Cannot open your file!");
        return;
    }
    while ((c = fgetc(fp)) != EOF) {
        if (c >= 'a' && c <= 'z')
            c -= 32;
        fputc(c, f);
    }
    fclose(fp);
    fclose(f);

    bytes_sent = send(sockfd, fileName, strlen(fileName), 0);
    if (bytes_sent < 0)
        perror("\nError: ");
    fp = fopen("res.txt", "rb");
    if (!fp) {
        printf("[!] Cannot open your file!");
        return;
    }
    while ((n = fread(buff, 1, BUFF_SIZE, fp)) > 0) {
        send(sockfd, buff, n, 0);
    }
    remove("res.txt");
    remove(fileName);

    close(sockfd);
}

char *upperCase(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        if (str[i] >= 'a' && str[i] <= 'z')
            str[i] -= 32;
    }
    return str;
}
