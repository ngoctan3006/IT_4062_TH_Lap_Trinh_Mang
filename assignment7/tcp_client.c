#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFF_SIZE 1024

int checkFileExtension(char *path);
char *getFilename(char *path);
int repeat();

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Usage: ./client IPAddress PortNumber\n");
        return -1;
    }
    int client_sock, total_bytes = 0;
    char path[BUFF_SIZE + 1], buff[BUFF_SIZE + 1];
    struct sockaddr_in server_addr; /* server's address information */
    int bytes_sent, bytes_received;

    //Step 1: Construct socket
    client_sock = socket(AF_INET, SOCK_STREAM, 0);

    //Step 2: Specify server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    //Step 3: Request to connect server
    if (connect(client_sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) < 0) {
        printf("\nError!Can not connect to sever! Client exit imediately! ");
        return 0;
    }

    //Step 4: Communicate with server

    //send message
    printf("\nEnter path of file to send: ");
    memset(path, '\0', (strlen(path) + 1));
    scanf("%s", path);
    while (getchar() != '\n');

    if (!checkFileExtension(path)) {
        printf("\n[!] File name extension must be \"txt\"\n\n");
        return 1;
    }
    FILE *fp = fopen(path, "rb");
    if (!fp) {
        printf("\n[!] Cannot open your file!\n\n");
        return 2;
    }

    char *filename = getFilename(path);
    char size[11];
    sprintf(size, "%10d", (int) strlen(filename));
    bytes_sent = send(client_sock, size, strlen(size), 0);
    if (bytes_sent <= 0) {
        printf("\nConnection closed!\n");
        return 3;
    }
    bytes_sent = send(client_sock, filename, strlen(filename), 0);
    if (bytes_sent <= 0) {
        printf("\nConnection closed!\n");
        return 3;
    }
    free(filename);

    int n;
    while ((n = fread(buff, 1, BUFF_SIZE, fp)) > 0) {
        send(client_sock, buff, n, 0);
        total_bytes += n;
    }
    strcpy(buff, "done");
    send(client_sock, buff, strlen(buff), 0);
    printf("\nSuccess: Sent %d bytes to server\n", total_bytes);

    //receive echo reply
    char tempFileName[BUFF_SIZE + 1];
    bytes_received = recv(client_sock, tempFileName, sizeof(filename), 0);
    if (bytes_received < 0)
        perror("\nError: ");
    else if (bytes_received == 0)
        printf("Connection closed.\n");
    mkdir("recv", S_IRWXU);

    tempFileName[bytes_received] = '\0';
    char fileName[BUFF_SIZE + 1] = "recv/";
    strcat(fileName, tempFileName);
    fp = fopen(fileName, "wb");
    if (!fp) {
        printf("\n[!] Cannot open your file!\n\n");
        return 2;
    }
    while ((n = recv(client_sock, buff, BUFF_SIZE, 0)) > 0) {
        fwrite(buff, sizeof(char), n, fp);
    }
    printf("\nReceived file from server successfully!");
    printf("\nThe file has been saved in the folder \"recv\" with the name \"%s\"\n\n", tempFileName);
    fclose(fp);

    //Step 4: Close socket
    close(client_sock);
    return 0;
}

int checkFileExtension(char *path) {
    char temp[100];
    int len = strlen(path);
    while (len > 0 && path[len - 1] != '.')
        len--;
    strcpy(temp, &path[len]);
    if (strcmp(temp, "txt") == 0)
        return 1;
    return 0;
}

char *getFilename(char *path) {
    int len = strlen(path);
    char *temp = (char *)malloc(BUFF_SIZE + 1);
    while (len > 0 && path[len - 1] != '/')
        len--;
    strcpy(temp, &path[len]);
    len = strlen(temp);
    while (len > 0 && temp[len - 1] != '.')
        len--;
    temp[len - 1] = '\0';
    return temp;
}
