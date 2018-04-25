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
char** tokeniza(char *entrada, int *argssize);
int verificaSimbolos(char **args, int argssize);
int isCommand(char *caminho);
int isRedirectCorrect(char **args, int *argssize);
char* getComando(char **agrs);
char** getArgumentos(char **args, int argssize);
char* getRedirecionamento(char **args, int argssize);
char* getArquivo(char **args, int argssize);
char* getSimbolos(char **args, int argssize);