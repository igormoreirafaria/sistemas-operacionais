#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#define SHELLMARK "$ "
#define true 1
#define false 0

void tokeniza(char*, char***, int*, char***, int*, int*);
int isRedirectCorrect(char **, int *);
char* getRedirecionamento(char **, int size);
int isCommand(char *);
char *getBackground(char **, int size);
