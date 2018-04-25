#include "shellso.h"

char** tokeniza(char *entrada, int *argssize){
	char**args;
	char *copiaSegura = (char*)malloc(strlen(entrada)*sizeof(char));
	strcpy(copiaSegura, entrada);
	char *token;
	*argssize=0;
	token = strtok(copiaSegura, " ");
	while(token != NULL){
		*argssize += 1;
		token = strtok(NULL, " ");
	}
	
	
	strcpy(copiaSegura, entrada);
	args = (char**)malloc(*argssize*sizeof(char*));
	
	token = strtok(copiaSegura, " ");
	int i = 0;
	while(token != NULL){
		args[i] = (char*)malloc(strlen(token)*sizeof(char));
		strcpy(args[i], token);
		token = strtok(NULL, " ");
		i++;
	}
	

	return args;
}

int verificaSimbolos(char **args, int argssize) {
	for(int i = 0; i < argssize; i++){
		if(strcmp(args[i], "=>") == 0 || strcmp(args[i], "<=") == 0 || strcmp(args[i], "|") == 0 || strcmp(args[i], "&") == 0 ){
			return i;
		}
	}
	return 0;
}

char* getSimbolos(char **args, int argssize) {
	for(int i = 0; i < argssize; i++){
		if(strcmp(args[i], "=>") == 0 || strcmp(args[i], "<=") == 0 || strcmp(args[i], "|") == 0 || strcmp(args[i], "&") == 0 ){
			return args[i];
		}
	}
	return NULL;
}

int isCommand(char *caminho){
	FILE *f;
	if(fopen(caminho, "r")){
		return true;
	}else{
		return false;
	}
}


int isRedirectCorrect(char **args, int *argssize){
	for (int i = 0; i < *argssize; i++){
		if (!strcmp(args[i], "<") || !strcmp(args[i], ">")){
			return false;
		} 
	}
	return true;
}

char* getComando(char** args){
	return args[0];
}

char ** getArgumentos(char **args, int argssize){
	int simboloIndex = verificaSimbolos(args, argssize);
	if(simboloIndex != 0){
		if(strcmp(args[simboloIndex],"=>")){
			char **argumentos = (char**)malloc(simboloIndex * sizeof(char*));
			for(int i = 0; i < simboloIndex; i++){
				argumentos[i] = args[i];
			}
			return argumentos;	
		}else
		if(strcmp(args[simboloIndex],"<=")){
			char **argumentos = (char**)malloc(simboloIndex+1 * sizeof(char*));
			for(int i = 0; i < simboloIndex; i++){
				argumentos[i] = args[i];
			}
			argumentos[simboloIndex+1] = NULL;
			return argumentos;
		}
	}else{
		char **argumentos = (char**)malloc(argssize * sizeof(char*));
		for(int i = 0; i< argssize; i++){
			argumentos[i] = args[i];
		}
		return argumentos;
	}

	
}

char* getRedirecionamento(char **args, int argssize){
	for(int i = 0; i < argssize; i++){
		if(strcmp(args[i], "=>") == 0 || strcmp(args[i], "<=") == 0 || strcmp(args[i], "|") == 0 || strcmp(args[i], "&") == 0 ){
			return args[i];
		}
	}
	return NULL;
}

char* getArquivo(char **args, int argssize){
	for(int i = 0; i < argssize; i++){
		if(strcmp(args[i], "=>") == 0 || strcmp(args[i], "<=") == 0 || strcmp(args[i], "|") == 0 || strcmp(args[i], "&") == 0 ){
			return args[i+1];
		}
	}
	return NULL;
}