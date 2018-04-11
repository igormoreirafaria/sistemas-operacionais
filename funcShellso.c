#include "shellso.h"

char** tokeniza(char *comando, char **args, int *agrssize){

	int i=0, spaceCount=0;

	char *token;
	//tratamento caso haja espaco no final da string
	if(comando[strlen(comando)-1] == ' '){
		comando[strlen(comando)-1] = '\0';
	}
	//conta quantos espacos ha na string
	while(comando[i]){
		if(isspace(comando[i])) spaceCount++;
		i++;
	}
	*agrssize = spaceCount + 1;
	args = (char**)malloc((spaceCount * sizeof(char*))+1);

	//tokeniza a string salvando os tokens no vetor args
	token = strtok(comando, " ");
	i = 0;
	while(token != NULL){
		args[i] = (char*)malloc(strlen(token)*sizeof(char));
		args[i] = token;
		token = strtok(NULL, " ");
		i++;
	}
	return args;
}

int isCommand(char *caminho){
	FILE *f;
	if(fopen(caminho, "r")){
		return true;
	}else{
		return false;
	}
}

int isRedirect(char **args, int *agrssize){
	for (int i = 0; i < *agrssize; ++i){
		if (!strcmp(args[i], "<=")){
			return 1;
		}else 
		if(!strcmp(args[i], "=>")){
			return 2;
		}
		else{
			return false;
		}
	}
}

int isRedirectCorrect(char **args, int *agrssize){
	for (int i = 0; i < *agrssize; i++){
		if (!strcmp(args[i], "<") || !strcmp(args[i], ">")){
			return false;
		} 
	}
	return true;
}