#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "studentmanager.h"

int main() {
    int choice;
    do {
        showMenu();
        printf("Enter your choice (1-5, other to quit): ");
        scanf("%d", &choice);
        clearStdin();

        switch(choice) {
            case 1:
                addNewScoreBoard();
                break;
            case 2:
                addScore();
                break;
            case 3:
                removeScore();
                break;
            case 4:
                searchScore();
                break;
            case 5:
                displayScore();
                break;
            default: break;
        }
    } while (choice >=1 && choice <= 5);
    return 0;
}