#include <stdio.h>
#include <string.h>
#include "hazchem.h"

int main() {
    char hazchemCode[MAX_LEN];
    char cf[MAX_LEN];
    int isContainColor;
    printf("Enter HAZCHEM code: ");
    scanf("%s", hazchemCode);
    while(getchar() != '\n');
    if(!checkHCCode(hazchemCode)) {
        printf("The code you entered is not a HAZCHEM code!\n");
        return 2;
    }

    isContainColor = containColor(hazchemCode);
    if(isContainColor) {
        do {
            printf("Is the %c reverse coloured? [yes/no] ", hazchemCode[1]);
            scanf("%s", cf);
            while(getchar() != '\n');
        } while(strcmp(cf, "yes") != 0 && strcmp(cf, "no") != 0);
    }

    printNotify(hazchemCode, cf);
    return 0;
}