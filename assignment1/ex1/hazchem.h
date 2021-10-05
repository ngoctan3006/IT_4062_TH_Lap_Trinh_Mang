#ifndef __HAZCHEM_H__
#define __HAZCHEM_H__

#define MAX_LEN 4
#define MAX_MATERIAL 10

int checkHCCode(char *code);
int containColor(char *code);
void printNotify(char *code, char *cf);
char *material(char *code);
char *reactivity(char *code);
char *protection(char *code, char *cf);
char *containment(char *code);

#endif // __HAZCHEM_H__