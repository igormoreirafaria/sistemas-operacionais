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
typedef struct x{
 char palavras[100];

}lista_comando;
void tokeniza(char *comando, char **args, char **redi, int *argssize);

int isCommand(char *caminho);
int isRedirectCorrect(char **args, int *agrssize);