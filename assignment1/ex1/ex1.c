#include <stdio.h>
#include <string.h>

#define MAX_LEN 4
#define MAX_MATERIAL 10

int checkHCCode(char *code);
int containColor(char *code);
void printNotify(char *code, char *cf);
char *material(char *code);
char *reactivity(char *code);
char *protection(char *code, char *cf);
char *containment(char *code);

int main() {
    char hazchemCode[MAX_LEN];
    char cf[MAX_LEN];
    int isContainColor;
    if(!hazchemCode) {
        printf("Error to allocate memory!\n");
        return 1;
    }
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

int checkHCCode(char *code) {
    const char *charCode = "PRSTWXYZ";
    int cLen = strlen(charCode);
    int len = strlen(code);
    if(len < 2 || len > MAX_LEN-1) return 0;
    if(code[0] < '1' || code[0] > '4') return 0;
    if(len == 3 && code[2] != 'E') return 0;
    for(int i = 0; i < cLen; i++) {
        if(code[1] == charCode[i]) return 1;
    }
    return 0;
}

int containColor(char *code) {
    return (code[1] == 'S' || code[1] == 'T' || code[1] == 'Y' || code[1] == 'Z');
}

void printNotify(char *code, char *cf) {
    printf("\n*** Emergency action advice ***\n");
    printf("  %-11s : %s\n", "Material", material(code));
    if(strlen(reactivity(code)) > 0) {
        printf("  %-11s : %s\n", "Reactivity", reactivity(code));
    }
    printf("  %-11s : %s\n", "Protection", protection(code, cf));
    printf("  %-11s : %s\n", "Containment", containment(code));
    if(strlen(code) == 3) {
        printf("  %-11s : %s\n", "Evacuation", "consider evacuation");
    }
    printf("*******************************\n\n");
}

char *material(char *code) {
    switch(code[0]) {
        case '1': return "jets";
        case '2': return "fog";
        case '3': return "foam";
        case '4': return "dry agent";
    }
}

char *reactivity(char *code) {
    switch(code[1]) {
        case 'P':
        case 'S':
        case 'W':
        case 'Y':
        case 'Z': return "can be violently reactive";
        default: return "";
    }
}

char *protection(char *code, char *cf) {
    switch(code[1]) {
        case 'P':
        case 'R':
        case 'W':
        case 'X': return "full protective clothing must be worn";
        case 'S':
        case 'T':
        case 'Y':
        case 'Z':
            if(strcmp(cf, "yes") == 0) return "breathing apparatus, protective gloves for fire only";
            return "breathing apparatus";
    }
}

char *containment(char *code) {
    switch(code[1]) {
        case 'P':
        case 'R':
        case 'S':
        case 'T': return "the dangerous goods may be washed down to a drain with a large quantity of water";
        case 'W':
        case 'X':
        case 'Y':
        case 'Z': return "a need to avoid spillages from entering drains or water courses";
    }
}