#ifndef __STUDENTMANAGER_H__
#define __STUDENTMANAGER_H__

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

Student inputStudentData(ScoreBoard scoreBoard);
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
void printStudent(Student data);
void printScoreBoard(ScoreBoard scoreBoard);
void printReport(ScoreBoard scoreBoard);

#endif // !__STUDENTMANAGER_H__