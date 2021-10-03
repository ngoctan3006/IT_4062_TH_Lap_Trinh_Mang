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

typedef struct scoreBoard {
    char subjectID[10];
    char subject[50];
    char semester[10];
    int midtermRate;
    int finalRate;
    int numOfStudent;
    Node *head;
} ScoreBoard;

Student inputStudentData(void);
void upperCase(char *string);
char convertScore(double score);
double inputScore(void);

Node *createNode(Student data);
Node *addHead(Node *head, Student data);
Node *removeNode(Node *head, char *id);
Node *searchNode(Node *head, char *id);
void freeList(Node *head);

char *makeFileName(char *subjectID, char *semester);
char *makeFileReportName(char *filename);
void fprintStudentData(FILE *fp, Student data);
ScoreBoard getScoreBoard(char *filename);
Student getStudent(FILE *fp);
void trim(char *string);
void printFile(char *filename, ScoreBoard scoreBoard);
void printFileReport(char *filename, ScoreBoard scoreBoard);

void showMenu(void);
void clearStdin(void);
void addNewScoreBoard(void);
void addScore(void);
void removeScore(void);
void searchScore(void);
void displayScore(void);
int isRepeat(char *message);

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

void upperCase(char *string) {
    int len = strlen(string);
    for(int i = 0; i < len; i++) {
        if(isalpha(string[i])) {
            string[i] = toupper(string[i]);
        }
    }
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

void freeList(Node *head) {
    Node *temp = head;
    while(temp != NULL) {
        head = head->next;
        free(temp);
        temp = head;
    }
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
        printf("[!] Error to allocate memory!\n");
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

ScoreBoard getScoreBoard(char *filename) {
    ScoreBoard scoreBoard;
    strcpy(scoreBoard.subjectID, "NONE");
    FILE *fp = fopen(filename, "r");
    if(!fp) {
        return scoreBoard;
    }
    Student data;

    fscanf(fp, "%*[^|]s"); // get "SubjectID"
    fgetc(fp); // get '|'
    fscanf(fp, "%[^\n]s", scoreBoard.subjectID);
    fgetc(fp); // get '\n'
    fscanf(fp, "%*[^|]s"); // get "Subject"
    fgetc(fp); // get '|'
    fscanf(fp, "%[^\n]s", scoreBoard.subject);
    fgetc(fp); // get '\n'
    fgetc(fp); // get 'F'
    fgetc(fp); // get '|'
    fscanf(fp, "%d", &scoreBoard.midtermRate);
    fgetc(fp); // get '|'
    fscanf(fp, "%d", &scoreBoard.finalRate);
    fgetc(fp); // get '\n'
    fscanf(fp, "%*[^|]s"); // get "Semester"
    fgetc(fp); // get '|'
    fscanf(fp, "%[^\n]s", scoreBoard.semester);
    fgetc(fp); // get '\n'
    fscanf(fp, "%*[^|]s"); // get "StudentCount"
    fgetc(fp); // get '|'
    fscanf(fp, "%d", &scoreBoard.numOfStudent);
    fgetc(fp); // get '\n'

    for(int i = 0; i < scoreBoard.numOfStudent; i++) {
        data = getStudent(fp);
        scoreBoard.head = addHead(scoreBoard.head, data);
    }

    fclose(fp);
    return scoreBoard;
}

Student getStudent(FILE *fp) {
    Student data;

    fgetc(fp);  // get 'S'
    fgetc(fp);  // get '|'
    fscanf(fp, "%[^|]s", data.id);
    fgetc(fp);  // get '|'
    fscanf(fp, "%[^|]s", data.lastName);
    trim(data.lastName);
    fgetc(fp);  // get '|'
    fscanf(fp, "%[^|]s", data.firstName);
    trim(data.firstName);
    fgetc(fp);  // get '|'
    fscanf(fp, "%lf", &data.midtermScore);
    fgetc(fp);  // get space
    fgetc(fp);  // get '|'
    fscanf(fp, "%lf", &data.finalScore);
    fgetc(fp);  // get space
    fgetc(fp);  // get '|'
    fgetc(fp);  // get space
    fscanf(fp, "%c", &data.score);
    fgetc(fp);  // get space
    fgetc(fp);  // get '|'
    fgetc(fp);  // get '\n'

    return data;
}

void trim(char *string) {
    int count = 0;
    int len = strlen(string);
    while(count < len) {
        if(string[count] == ' ') count++;
        else break;
    }
    if(count) {
        strcpy(&string[0], &string[count]);
        len = strlen(string);
    }
    while(len > 0) {
        if(string[len-1] == ' ') len--;
        else break;
    }
    string[len] = '\0';
}

void printFile(char *filename, ScoreBoard scoreBoard) {
    FILE *fp = fopen(filename, "w");
    if(!fp) {
        printf("[!] Can't open file!\n");
        exit(2);
    }
    Student data;

    fprintf(fp, "SubjectID|%s\n", scoreBoard.subjectID);
    fprintf(fp, "Subject|%s\n", scoreBoard.subject);
    fprintf(fp, "F|%d|%d\n", scoreBoard.midtermRate, scoreBoard.finalRate);
    fprintf(fp, "Semester|%s\n", scoreBoard.semester);
    fprintf(fp, "StudentCount|%d\n", scoreBoard.numOfStudent);
    for(int i = 0; i < scoreBoard.numOfStudent; i++) {
        printf("  Enter information of student %d:\n", i+1);
        data = inputStudentData();
        data.totalScore = (scoreBoard.midtermRate * data.midtermScore + scoreBoard.finalRate * data.finalScore) / 100;
        if(data.midtermScore < 3 || data.finalScore < 3) {
            data.score = 'F';
        } else {
            data.score = convertScore(data.totalScore);
        }
        fprintStudentData(fp, data);
    }
    fclose(fp);
}

void printFileReport(char *filename, ScoreBoard scoreBoard) {
    FILE *fp = fopen(filename, "w");
    if(!fp) {
        printf("[!] Can't open file %s\n", filename);
        exit(2);
    }

    Student maxScore;
    Student minScore;
    double max = -1.0;
    double min = 11.0;
    int histogramData[6] = {0};
    double sum = 0.0;
    Node *temp = scoreBoard.head;
    while(temp != NULL) {
        if(max < temp->data.totalScore) {
            maxScore = temp->data;
        }
        if(min > temp->data.totalScore) {
            minScore = temp->data;
        }
        sum += temp->data.totalScore;
        histogramData[temp->data.score - 'A']++;
        temp = temp->next;
    }

    fprintf(fp, "%s\n\n\n", filename);
    fprintf(fp, "The student with the highest mark is: %s %s\n", maxScore.lastName, maxScore.firstName);
    fprintf(fp, "The student with the lowest mark is: %s %s\n", minScore.lastName, minScore.firstName);
    fprintf(fp, "The average mark is: %.2lf\n\n\n", sum / scoreBoard.numOfStudent);
    fprintf(fp, "A histogram of the subject %s is:\n", scoreBoard.subjectID);
    for(int i = 0; i < 6; i++) {
        if(i != 4) {
            fprintf(fp, "%c: ", i + 'A');
            for(int j = 0; j < histogramData[i]; j++) {
                fprintf(fp, "*");
            }
            fprintf(fp, "\n");
        }
    }
    
    fclose(fp);
}

void showMenu(void) {
    printf("\n");
    printf("  +----------------------------------------------+\n");
    printf("  | %-45s|\n", "Learning Management System");
    printf("  +---+------------------------------------------+\n");
    printf("  | 1 | %-40s |\n", "Add a new scoreboard");
    printf("  | 2 | %-40s |\n", "Add score");
    printf("  | 3 | %-40s |\n", "Remove score");
    printf("  | 4 | %-40s |\n", "Search score");
    printf("  | 5 | %-40s |\n", "Display scoreboard and score report");
    printf("  +---+------------------------------------------+\n\n");
}

void clearStdin(void) {
    while(getchar() != '\n');
}

void addNewScoreBoard(void) {
    ScoreBoard scoreBoard;
    do {
        printf("\nAdd new scoreboard:\n");
        printf("  Enter subject ID: ");
        scanf("%s", scoreBoard.subjectID);
        clearStdin();
        printf("  Enter subject name: ");
        scanf("%[^\n]s", scoreBoard.subject);
        clearStdin();
        printf("  Enter midterm rate: ");
        scanf("%d", &scoreBoard.midtermRate);
        clearStdin();
        printf("  Enter final rate: ");
        scanf("%d", &scoreBoard.finalRate);
        clearStdin();
        printf("  Enter semester: ");
        scanf("%s", scoreBoard.semester);
        clearStdin();
        printf("  Enter number of students: ");
        scanf("%d", &scoreBoard.numOfStudent);
        clearStdin();

        char *filename = makeFileName(scoreBoard.subjectID, scoreBoard.semester);
        printFile(filename, scoreBoard);
        free(filename);
    } while(isRepeat("Do you want to add another scoreboard"));
}

void addScore(void) {
    char subjectID[10];
    char semester[10];
    int numOfStudent;
    Student data;

    printf("\nAdd score\n");
    printf("  Enter subject ID: ");
    scanf("%s", subjectID);
    clearStdin();
    printf("  Enter semester: ");
    scanf("%s", semester);
    clearStdin();

    char *filename = makeFileName(subjectID, semester);
    ScoreBoard scoreBoard = getScoreBoard(filename);
    if(strcmp(scoreBoard.subjectID, "NONE") == 0) {
        printf("  [!] Scoreboard with subject ID '%s' is not exist!\n", subjectID);
        return;
    }
    numOfStudent = scoreBoard.numOfStudent;

    do {
        data = inputStudentData();
        numOfStudent++;
    } while(isRepeat("Do you want to add add another student"));

    free(filename);
    freeList(scoreBoard.head);
}

void removeScore(void) {}

void searchScore(void) {}

void displayScore(void) {}

int isRepeat(char *message) {
    char choice;
    do {
        printf("  %s? [y/n]: ", message);
        scanf("%c", &choice);
        clearStdin();
        if(isalpha(choice)) {
            choice = tolower(choice);
        }
    } while (choice != 'y' && choice != 'n');
    if(choice == 'y') return 1;
    else return 0;
}