#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>

#define BUFF_SIZE 1024

int checkFileExtension(char *path);

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Usage: ./client IPAddress PortNumber\n");
        return -1;
    }
    int client_sock, total_bytes = 0;
    char path[BUFF_SIZE + 1], buff[BUFF_SIZE + 1], filename[BUFF_SIZE + 1];
    struct sockaddr_in server_addr; /* server's address information */
    int msg_len, bytes_sent, bytes_received;

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
    while(getchar() != '\n');

    if(!checkFileExtension(path)) {
        printf("[!] File name extension must be \"txt\"\n");
        return 1;
    }
    FILE *fp = fopen(path, "r");
    if(!fp) {
        printf("[!] Cannot open your file!");
        return 2;
    }

    int n;
    while ((n = fread(buff, 1, BUFF_SIZE, fp)) > 0) {
        send(client_sock, buff, n, 0);
        total_bytes += n;
    }
    printf("\nSuccess: Sent %d bytes to server\n", total_bytes);

    //receive echo reply
    bytes_received = recv(client_sock, filename, BUFF_SIZE, 0);
    if (bytes_received < 0)
        perror("\nError: ");
    else if (bytes_received == 0)
        printf("Connection closed.\n");
    mkdir("recv", S_IRWXU);

    filename[bytes_received] = '\0';
    fp = fopen(strcat("recv/", filename), "wb");
    if(!fp) {
        printf("[!] Cannot open your file!");
        return 2;
    }
    while ((n = recv(client_sock, buff, BUFF_SIZE, 0)) > 0) {
        fwrite(buff, sizeof(char), n, fp);
    }
    printf("\nReceived file from server successfully!");
    printf("\nThe file has been saved in the folder \"recv\" with the name \"%s\"\n", filename);
    fclose(fp);

    //Step 4: Close socket
    close(client_sock);
    return 0;
}

int checkFileExtension(char *path) {
    char temp[100];
    int len = strlen(path);
    while(len > 0 && path[len - 1] != '.')
        len--;
    strcpy(temp, &path[len]);
    if (strcmp(temp, "txt") == 0)
        return 1;
    return 0;
}
