#include <stdio.h> /* These are the usual header files */
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#define BACKLOG 5 /* Number of allowed connections */
#define BUFF_SIZE 1024
#define MAX_SIZE 256
#define FILENAME "account.txt" // File containing account information

typedef struct account {
    char username[MAX_SIZE];
    char password[MAX_SIZE];
    int status;
    int time;
} Account;

typedef struct node {
    Account account;
    struct node *next;
} Node;

Node *createNode(Account account);
Node *addHead(Node *head, Account account);
Node *searchNode(Node *head, char *username);
void freeList(Node *head);

Node *readFile(char *filename);
Account getAccount(char *string);
int checkAccount(Node *head, char *username, char *password);
void exportFile(Node *head, char *filename);

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: ./server PortNumber\n");
        return -1;
    }
    int listen_sock, conn_sock; /* file descriptors */
    char recv_data[BUFF_SIZE];
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

    // Step 4: Communicate with client
    while (1) {
        // accept request
        sin_size = sizeof(struct sockaddr_in);
        if ((conn_sock = accept(listen_sock, (struct sockaddr *)&client, &sin_size)) == -1)
            perror("\nError ");

        printf("You got a connection from %s\n", inet_ntoa(client.sin_addr)); /* prints client's IP */

        Node *head = readFile(FILENAME);
        Node *find = NULL;
        Account account;
        int isCorrect = 0;
        char reply[MAX_SIZE];
        char currUser[MAX_SIZE/2];
        while (1) {
            bytes_received = recv(conn_sock, recv_data, BUFF_SIZE - 1, 0);
            if (bytes_received <= 0) {
                printf("\nConnection closed");
                break;
            }
            recv_data[bytes_received] = '\0';
            if (strcmp(recv_data, "Bye") == 0) {
                sprintf(reply, "Goodbye %s", currUser);
            } else {
                account = getAccount(recv_data);
                isCorrect = checkAccount(head, account.username, account.password);
                switch (isCorrect) {
                    case -2:
                    case -1:
                        strcpy(reply, "0"); // Account is blocked or inactive
                        break;
                    case 0:
                        find = searchNode(head, account.username);
                        find->account.time++;
                        if(find->account.time < 3) {
                            strcpy(reply, "1"); // Password is not correct
                        } else {
                            strcpy(reply, "2"); // Account is blocked
                            find->account.status = 0;
                            exportFile(head, FILENAME);
                        }
                        break;
                    case 1:
                        strcpy(reply, "3"); // Login is successful
                        strcpy(currUser, account.username);
                        break;
                }
            }

            bytes_sent = send(conn_sock, reply, strlen(reply), 0);
            if (bytes_sent <= 0) {
                printf("\nConnection closed\n");
                break;
            }
        }
        freeList(head);
        close(conn_sock);
    }

    close(listen_sock);
    return 0;
}

Node *createNode(Account account) {
    Node *newNode = (Node *) malloc(sizeof(Node));
    newNode->account = account;
    newNode->next = NULL;
    return newNode;
}

Node *addHead(Node *head, Account account) {
    Node *newNode = createNode(account);
    if (head == NULL) return newNode;
    newNode->next = head;
    return newNode;
}

Node *searchNode(Node *head, char *username) {
    Node *temp = head;
    while (temp != NULL && strcmp(temp->account.username, username) != 0) {
        temp = temp->next;
    }
    return temp;
}

void freeList(Node *head) {
    Node *temp = head;
    while (temp != NULL) {
        head = head->next;
        free(temp);
        temp = head;
    }
}

Node *readFile(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("  [!] Cannot open file \"%s\"\n", filename);
        exit(1);
    }
    Node *head = NULL;
    Account account;
    while (!feof(fp)) {
        fscanf(fp, "%s %s %d\n", account.username, account.password, &account.status);
        account.time = 0;
        head = addHead(head, account);
    }
    fclose(fp);
    return head;
}

Account getAccount(char *string) {
    int len = strlen(string);
    Account account;
    strcpy(account.username, string);
    int i = 0;
    while (i < len) {
        if (account.username[i] == '\n') {
            account.username[i] = '\0';
            break;
        }
        i++;
    }
    strcpy(account.password, &string[i+1]);
    return account;
}

int checkAccount(Node *head, char *username, char *password) {
    Node *find = searchNode(head, username);
    if (!find) return -2; // account not exist
    if (strcmp(find->account.password, password) != 0) return 0; // password incorrect
    if (find->account.status == 0) return -1; // account is blocked
    return 1; // login successfully
}

void exportFile(Node *head, char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("  [!] Cannot open file \"%s\"\n", filename);
        exit(1);
    }
    Node *temp = head;
    while(temp) {
        fprintf(fp, "%s %s %d\n", temp->account.username, temp->account.password, temp->account.status);
        temp = temp->next;
    }
    fclose(fp);
}