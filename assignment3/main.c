#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void resolver(char *string);

int main(int argc, char *argv[]) {
    if(argc < 2) {
        printf("Syntax error!\n");
    } else {
        resolver(argv[1]);
    }
    return 0;
}

void resolver(char *string) {
    struct hostent *host;
    struct in_addr **addr_list;
    struct in_addr addr;
    if(inet_addr(string) == -1) {
        host = gethostbyname(string);
        if(!host) {
            printf("No Information found\n");
            return;
        }
        addr_list = (struct in_addr **) host->h_addr_list;
        printf("Official IP: %s\n", inet_ntoa(*addr_list[0]));
        printf("Alias IP:\n");
        if(addr_list[1] == NULL) {
            printf("No aliases found\n");
        } else {
            for(int i = 1; addr_list[i]; i++) {
                printf("%s\n", inet_ntoa(*addr_list[i]));
            }
        }
    } else {
        inet_pton(AF_INET, string, &addr);
        host = gethostbyaddr(&addr, sizeof(addr), AF_INET);
        if(!host) {
            printf("No Information found\n");
            return;
        }
        printf("Official name: %s\n", host->h_name);
        printf("Alias name:\n");
        if(host->h_aliases[0] == NULL) {
            printf("No aliases found\n");
        } else {
            for(int i = 0; host->h_aliases[i] != NULL; i++) {
                printf("%s\n", host->h_aliases[i]);
            }
        }
    }
}