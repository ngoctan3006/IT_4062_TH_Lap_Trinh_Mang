#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct student {
    char id[10];
    char firstName[20];
    char lastName[30];
    double midtermScore;
    double finalScore;
    double totalScore;
    char score;
} Student;

typedef struct node {
    Student data;
    struct node *next;
} Node;

Student inputStudentData(void);
char *upperCase(char *string);
char convertScore(double score);
double inputScore(void);

Node *createNode(Student data);
Node *addHead(Node *head, Student data);
Node *removeNode(Node *head, char *id);
Node *searchNode(Node *head, char *id);

char *makeFileName(char *subjectID, char *semester);
char *makeFileReportName(char *filename);
void fprintStudentData(FILE *fp, Student data);

void showMenu(void);
void clearStdin(void);
void addNewScoreBoard(void);
void addScore(void);
void removeScore(void);
void searchScore(void);
void displayScore(void);
int isRepeat(void);

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

Student inputStudentData(void) {
    Student data;
    printf("  Enter student ID: ");
    scanf("%s", data.id);
    clearStdin();
    printf("  Enter first name: ");
    scanf("%[^\n]s", data.firstName);
    clearStdin();
    upperCase(data.firstName);
    printf("  Enter last name: ");
    scanf("%[^\n]s", data.lastName);
    clearStdin();
    upperCase(data.lastName);
    printf("  Enter midterm score: ");
    data.midtermScore = inputScore();
    printf("  Enter final score: ");
    data.finalScore = inputScore();

    return data;
}

char *upperCase(char *string) {
    int len = strlen(string);
    for(int i = 0; i < len; i++) {
        if(isalpha(string[i])) {
            string[i] = toupper(string[i]);
        }
    }
    return string;
}

char convertScore(double score) {
    if(score >= 8.5) return 'A';
    if(score >= 7.0) return 'B';
    if(score >= 5.5) return 'C';
    if(score >= 4.0) return 'D';
    return 'F';
}

double inputScore(void) {
    double score;
    do {
        scanf("%lf", &score);
        clearStdin();
        if(score < 0 || score > 10) {
            printf("  Score must be between 0 and 10!\n  Enter again: ");
        }
    } while(score < 0 || score > 10);
    return score;
}

Node *createNode(Student data) {
    Node *newNode = (Node *) malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

Node *addHead(Node *head, Student data) {
    Node *newNode = createNode(data);
    if(head == NULL) return newNode;
    newNode->next = head;
    return newNode;
}

Node *removeNode(Node *head, char *id) {
    if(head == NULL) return NULL;
    Node *curr = head;
    Node *prev = NULL;
    if(strcmp(head->data.id, id) == 0) {
        head = head->next;
        free(curr);
        return head;
    }
    while(curr != NULL && strcmp(curr->data.id, id) != 0) {
        prev = curr;
        curr = curr->next;
    }
    if(curr == NULL) return head;
    prev->next = curr->next;
    free(curr);
    return head;
}

Node *searchNode(Node *head, char *id) {
    Node *temp = head;
    while(temp != NULL && strcmp(temp->data.id, id) != 0) {
        temp = temp->next;
    }
    return temp;
}

char *makeFileName(char *subjectID, char *semester) {
    int size = strlen(subjectID) + strlen(semester) + strlen("_.txt") + 1;
    char *filename = (char *) malloc(size * sizeof(char));
    if(filename == NULL) {
        printf("[!] Error to allocate memory!\n");
        exit(1);
    }
    strcpy(filename, subjectID);
    strcat(filename, "_");
    strcat(filename, semester);
    strcat(filename, ".txt");
    return filename;
}

char *makeFileReportName(char *filename) {
    int size = strlen(filename) + strlen("_rp") + 1;
    char *fileReportName = (char *) malloc(size * sizeof(char));
    if(fileReportName == NULL) {
        printf("Error to allocate memory!\n");
        exit(1);
    }
    strncpy(fileReportName, filename, strlen(filename) - 4);
    strcat(fileReportName, "_rp.txt");
    return fileReportName;
}

void fprintStudentData(FILE *fp, Student data) {
    fprintf(fp, "S|%-8s|", data.id);
    fprintf(fp, "%-15s|", data.lastName);
    fprintf(fp, "%-7s|", data.firstName);
    fprintf(fp, " %4.1lf |", data.midtermScore);
    fprintf(fp, " %4.1lf |", data.finalScore);
    fprintf(fp, " %c |\n", data.score);
}

void showMenu(void) {
    printf("\n");
    printf("  +----------------------------------------------+\n");
    printf("  | %-45s|\n", "Learning Management System");
    printf("  +---+------------------------------------------+\n");
    printf("  | 1 | %-40s |\n", "Add a new score board");
    printf("  | 2 | %-40s |\n", "Add score");
    printf("  | 3 | %-40s |\n", "Remove score");
    printf("  | 4 | %-40s |\n", "Search score");
    printf("  | 5 | %-40s |\n", "Display score board and score report");
    printf("  +---+------------------------------------------+\n\n");
}

void clearStdin(void) {
    while(getchar() != '\n');
}

void addNewScoreBoard(void) {
    char subjectID[10];
    char subject[50];
    char semester[10];
    int midtermRate, finalRate;
    int numOfStudent;
    Student data;

    do {
        printf("\nAdd new score board:\n");
        printf("  Enter subject ID: ");
        scanf("%s", subjectID);
        clearStdin();
        printf("  Enter subject name: ");
        scanf("%[^\n]s", subject);
        clearStdin();
        printf("  Enter midterm rate: ");
        scanf("%d", &midtermRate);
        clearStdin();
        printf("  Enter final rate: ");
        scanf("%d", &finalRate);
        clearStdin();
        printf("  Enter semester: ");
        scanf("%s", semester);
        clearStdin();
        printf("  Enter number of students: ");
        scanf("%d", &numOfStudent);
        clearStdin();

        char *filename = makeFileName(subjectID, semester);
        FILE *fp = fopen(filename, "w");
        if(fp == NULL) {
            printf("[!] Can't open file!\n");
            exit(2);
        }

        fprintf(fp, "SubjectID|%s\n", subjectID);
        fprintf(fp, "Subject|%s\n", subject);
        fprintf(fp, "F|%d|%d\n", midtermRate, finalRate);
        fprintf(fp, "Semester|%s\n", semester);
        fprintf(fp, "StudentCount|%d\n", numOfStudent);
        for(int i = 0; i < numOfStudent; i++) {
            printf("  Enter information of student %d:\n", i+1);
            data = inputStudentData();
            data.totalScore = (midtermRate * data.midtermScore + finalRate * data.finalScore) / 100;
            if(data.midtermScore < 3 || data.finalScore < 3) {
                data.score = 'F';
            } else {
                data.score = convertScore(data.totalScore);
            }
            fprintStudentData(fp, data);
        }

        free(filename);
        fclose(fp);
    } while(isRepeat());
}

void addScore(void) {}

void removeScore(void) {}

void searchScore(void) {}

void displayScore(void) {}

int isRepeat(void) {
    char choice;
    do {
        printf("  Do you want to repeat? [y/n]: ");
        scanf("%c", &choice);
        clearStdin();
        if(isalpha(choice)) {
            choice = tolower(choice);
        }
    } while (choice != 'y' && choice != 'n');
    if(choice == 'y') return 1;
    else return 0;
}