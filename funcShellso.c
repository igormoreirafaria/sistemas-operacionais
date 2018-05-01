#include "shellso.h"

void tokeniza( char *entrada, char ***args, int *argsSize, char ***args2, int *args2size, int *pipeFlags ){
	char *token;

	char *copia = ( char* )calloc( strlen(entrada), sizeof(char) );
	strcpy( copia, entrada );

	int i = 0, j = 0;

	int pipeFlag = 0;

	token = strtok( copia, " ");
	while( token != NULL ) {
    	
		if( !strcmp( token, "|" ) ){
			pipeFlag = 1;
			token = strtok( NULL, " " );
			continue;	
		} 


    	if( pipeFlag == 0 ){
    		i++;
    	}else if( pipeFlag == 1){
    		j++;
    	}
    	
    	token = strtok( NULL, " " );
	}

	*argsSize = i;
	(*args) = ( char** )calloc( (*argsSize) , sizeof(char*) );
	
	if( pipeFlag == 1){
		*args2size = j;
		(*args2) = ( char** )calloc( (*args2size) , sizeof(char*) );	
	}

	pipeFlag = 0;
	i = 0;
	j = 0;

	token = strtok( entrada, " ");
	while( token != NULL ) {
    	
		if( !strcmp( token, "|" ) ){
			pipeFlag = 1;
			token = strtok( NULL, " " );
			continue;	
		} 


    	if( pipeFlag == 0 ){
    		
    		(*args)[i] = ( char* )calloc( (strlen(token)) , sizeof(char) );
    		strcpy( (*args)[i], token );
    		i++;
    	}else if( pipeFlag == 1){
    		
    		(*args2)[j] = ( char* )calloc( (strlen(token)) , sizeof(char) );
    		strcpy( (*args2)[j], token );
    		j++;
    	}
    	
    	token = strtok( NULL, " " );
	}

	*pipeFlags = pipeFlag;
}

int isRedirectCorrect(char **args, int *argssize){
	for (int i = 0; i < *argssize; i++){
		if (!strcmp(args[i], "<") || !strcmp(args[i], ">")){
			return false;
		} 
	}
	return true;
}

char* getRedirecionamento(char **args, int argssize){
	for(int i = 0; i < argssize; i++){
		if(strcmp(args[i], "=>") == 0 || strcmp(args[i], "<=") == 0 ){
			return args[i];
		}
	}
	return NULL;
}

char* getBackground(char **args, int argssize){
	for(int i = 0; i < argssize; i++){
		if(strcmp(args[i], "&") == 0 ){
			return args[i];
		}
	}
	return NULL;
}

int isCommand(char *caminho){
	if( access( caminho, F_OK ) != -1 ) {
    	// file exists
		return true;
	} else {
	    // file doesn't exist
		return false;
	}
}
