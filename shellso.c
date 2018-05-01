#include "shellso.h"	

int main(int argc, char **argv[]){

 	int cp[2]; /* Child to parent pipe */
 	int cp2[2]; /* Child to parent pipe */
	int pid;
	char *entrada;
	
	char **args;
	char **args2;
	int argsSize = 0, args2size = 0;

	int pipeFlag;
	char* redirecionamento;
	char* redirecionamento2;
	char* background;
	char *caminho;
	char *caminho2;
	char *filepath;
	char *filepath2;
	int filedesc;
	char *file;
	char *file2;
	int ch;
	printf("Digite seu comando ou digite quit pra sair\n");
	
	while(true){
		entrada = (char*)calloc(1024, sizeof(char));
		if( pipe(cp) < 0){
			perror("Can't make pipe");
			exit(1);
    	}
    	if( pipe(cp2) < 0){
			perror("Can't make pipe");
			exit(1);
    	}

    	//imprime $ na tela
		printf(SHELLMARK);
		//pega a entrada do terminal
		fgets(entrada, 1024, stdin); //pega a entrada do terminal
		if(!strcmp(entrada, "quit\n")) break;
		//caso o usuario digite enter o loop salta pra proxima iteracao
		if(!strcmp(entrada, "\n")) continue;
		//remove o \n da string
		if(entrada[strlen(entrada)-1] == '\n'){ 
			entrada[strlen(entrada)-1] = '\0';
		}

		tokeniza(entrada, &args, &argsSize, &args2, &args2size, &pipeFlag);
		/*
		for (int i = 0; i < argsSize; ++i){
			printf("%s\n", args[i]);
		}

		if( pipeFlag == 1 ){
			for (int i = 0; i < args2ize; ++i){
				printf("%s\n", args2[i]);
			}
		}*/
		
		if(!isRedirectCorrect(args, &argsSize)){
			printf("Nao e possivel utilizar este simbolo para redirecionar a saida do programa.\n");
			continue;
		}
		if(!isRedirectCorrect(args2, &args2size)){
			printf("Nao e possivel utilizar este simbolo para redirecionar a saida do programa.\n");
			continue;
		}

		redirecionamento = getRedirecionamento(args, argsSize);
		background = getBackground(args, argsSize);
		caminho = (char*)calloc(1024,sizeof(char));
		sprintf(caminho, "/bin/%s", args[0]);



		//verifica se o comando e valido
		if(!isCommand(caminho)){
			printf("Comando %s nao valido\n", args[0]);
		}else{

			if(redirecionamento == NULL){
				if(pipeFlag == 0){
					args = (char**)realloc( args, (argsSize + 1) * sizeof(char*) );
					args[argsSize+1] = NULL;

					switch( pid = fork() ){
				        case -1: 
				            perror("Can't fork");
				            exit(1);
				        case 0:
				        	execvp(caminho, args);
		        			perror("No exec");
		            		exit(1);

		            	default:
		            		if(background == NULL){
		            			waitpid(pid, NULL, WCONTINUED);
		            		}
					}
				}else if(pipeFlag == 1){
					if( pipe(cp2) < 0){
						perror("Can't make pipe");
						exit(1);
			    	}
						
					args = (char**)realloc( args, (argsSize + 1) * sizeof(char*) );
					args[argsSize+1] = NULL;
					switch( pid = fork() ){
				        case -1: 
				            perror("Can't fork");
				            exit(1);
				        case 0:
				        	close(cp[0]);
				        	dup2(cp[1], 1);
				        	execvp(caminho, args);
		        			perror("No exec");
		            		exit(1);

		            	default:
		            		close(cp[1]);
		            		waitpid(pid, NULL, WCONTINUED);
		            		
					}

					redirecionamento2 = getRedirecionamento(args2, args2size);
					caminho2 = (char*)calloc(1024, sizeof(char));
					background = getBackground(args2, args2size);
					sprintf(caminho2, "/bin/%s", args2[0]);
					if(!isCommand(caminho2)){
						printf("Comando %s nao valido\n", args2[0]);
					}else{
						if(redirecionamento2 == NULL){
							
							args2 = (char**)realloc( args2, (args2size + 1) * sizeof(char*) );
							args2[argsSize+1] = NULL;

							switch( pid = fork() ){
						        case -1: 
						            perror("Can't fork");
						            exit(1);
						        case 0:
						        	close(cp[1]);
						        	dup2(cp[0],0);
						        	execvp(caminho2, args2);
				        			perror("No exec");
				            		exit(1);

				            	default:
				            		close(cp[0]);
				            		if(background == NULL){
				            			waitpid(pid, NULL, WCONTINUED);
				            		}
							}
						}else if(redirecionamento2 != NULL){


							if(strcmp(redirecionamento2, "=>")==0){
					
								for (int i = 0; i < args2size; ++i){
									if(!strcmp( args2[i], "=>" )){
										file2 = (char*)calloc( (strlen(args2[i+1])) , sizeof(char) );
										strcpy( file2, args2[i+1] );
										args2[i] = NULL;
										break;
									}
								}

								
							    switch( pid = fork() ){
							        case -1: 
							            perror("Can't fork");
							            exit(1);
							        case 0:

							        	close(cp[1]);
							        	close(cp2[0]);
							        	
							        	dup2(cp2[1],1);
							        	dup2(cp[0],0);

							        	execvp(caminho2, args2);
					        			perror("No exec");
					            		exit(1);

					            	default:
					            		
					            		waitpid(pid, NULL, WCONTINUED);
										close(cp2[1]);
								}
								
								
						    	switch(pid = fork()) {
							        case -1: 
							            perror("Can't fork");
							            exit(1);
							        case 0: 
										//define o caminho para o arquivo de saida
										close(cp2[1]);
										close(cp[1]);
										close(cp[0]);
										filepath2 = (char*)calloc(1024 , sizeof(char));
										sprintf(filepath2,"/home/igor/Documents/sistemas-operacionais/%s", file2);
										
										filedesc = open(filepath2, O_CREAT | O_WRONLY, 0777);
										
										while(read(cp2[0], &ch, 1)>0) {
											write(filedesc, &ch, 1);
										}
					            		close( cp2[0]);
					            		close(filedesc);
					            		free(filepath2);
					            		exit(1);
					            	default:
					            		if(background == NULL){
					            			waitpid(pid, NULL, WCONTINUED);
					            		}
					            		
								}
								free(file2);

								for (int i = 0; i < args2size; ++i){
									if(args2[i] == NULL){
										char *bla = (char*)calloc(strlen("=>") , sizeof(char));
										strcpy(bla, "=>");
										args2[i] = bla;
										free(bla);
									}
								}
							}
						}
					}
				}
			}else if(redirecionamento != NULL){
				
				filepath = (char*)calloc(1024 , sizeof(char));

				if(strcmp(redirecionamento, "=>")==0){

					for (int i = 0; i < argsSize; ++i){
						if(!strcmp( args[i], "=>" )){
							file = (char*)calloc( (strlen(args[i+1])) , sizeof(char) );
							strcpy( file, args[i+1] );
							args[i] = NULL;
							break;
						}
					}
						
					
				    switch( pid = fork() ){
				        case -1: 
				            perror("Can't fork");
				            exit(1);
				        case 0:

		            		close( cp[0]);
				        	dup2(cp[1], 1);

		            		execvp(caminho, args);
		        			perror("No exec");
		            		exit(1);

		            	default:
		            		
		            		waitpid(pid, NULL, WCONTINUED);
							close(cp[1]);
					}
					
					
			    	switch(pid = fork()) {
				        case -1: 
				            perror("Can't fork");
				            exit(1);
				        case 0: 
							close(cp[1]);
							//define o caminho para o arquivo de saida
							sprintf(filepath,"/home/igor/Documents/sistemas-operacionais/%s", file);
							
							
				       		int filedesc = open(filepath, O_CREAT | O_WRONLY, 0777);
							
							while(read(cp[0], &ch, 1)>0) {
								
								write(filedesc, &ch, 1);
							}
		            		close( cp[0]);
		            		
		            		exit(1);
		            	default:
		            		if(background == NULL){
		            			waitpid(pid, NULL, WCONTINUED);
		            		}
		            		
					}
					free(file);
					for (int i = 0; i < argsSize; ++i){
						if(args[i] == NULL){
							char *bla = (char*)calloc(strlen("=>") , sizeof(char));
							strcpy(bla, "=>");
							args[i] = bla;
							free(bla);
						}
					}
				}else if(strcmp(redirecionamento, "<=")==0){
					if(pipeFlag == 0){
						
						for (int i = 0; i < argsSize; ++i){
							if(!strcmp( args[i], "<=" )){
								file = (char*)calloc( (strlen(args[i+1])) , sizeof(char) );
								strcpy( file, args[i+1] );
								args[i] = NULL;
								break;
							}
						}

						switch(pid = fork()) {
					        case -1: 
					            perror("Can't fork");
					            exit(1);
					        case 0:
								close(cp[1]);
								close(cp[0]);
								close(cp2[1]);
								close(cp2[0]);
					        	sprintf(filepath, "/home/igor/Documents/sistemas-operacionais/%s", file);
								if(!isCommand(filepath)){
									printf("arquivo %s nao valido\n", file);
								}

								filedesc = open(filepath, O_RDONLY, 0777);
								dup2(filedesc,0);
								execvp(caminho, args);
								perror("No exec");
								exit(1);
								
								
			            	default:

			            		if(background == NULL){
			            			waitpid(pid, NULL, WCONTINUED);
			            		}
						}


						free(file);
						for (int i = 0; i < argsSize; ++i){
							if(args[i] == NULL){
								char *bla = (char*)calloc(strlen("<=") , sizeof(char));
								strcpy(bla, "<=");
								args[i] = bla;
							}
						}

					}else if(pipeFlag == 1){

						for (int i = 0; i < argsSize; ++i){
							if(!strcmp( args[i], "<=" )){
								file = (char*)calloc( (strlen(args[i+1])) , sizeof(char) );
								strcpy( file, args[i+1] );
								args[i] = NULL;
								break;
							}
						}

						switch(pid = fork()) {
					        case -1: 
					            perror("Can't fork");
					            exit(1);
					        case 0:
								dup2(cp[1], 1);
								close(cp[0]);
								close(cp2[1]);
								close(cp2[0]);
					        	sprintf(filepath, "/home/igor/Documents/sistemas-operacionais/%s", file);
								if(!isCommand(filepath)){
									printf("arquivo %s nao valido\n", file);
								}

								filedesc = open(filepath, O_RDONLY, 0777);
								dup2(filedesc,0);
								execvp(caminho, args);
								perror("No exec");
								exit(1);
								
								
			            	default:
			            		close(cp[1]);
			            		waitpid(pid, NULL, WCONTINUED);
						}

						free(file);
						for (int i = 0; i < argsSize; ++i){
							if(args[i] == NULL){
								char *bla = (char*)calloc(strlen("<=") , sizeof(char));
								strcpy(bla, "<=");
								args[i] = bla;
							}
						}
						background = getBackground(args2, args2size);
						redirecionamento2 = getRedirecionamento(args2, args2size);
						caminho2 = (char*)calloc(1024, sizeof(char));
						sprintf(caminho2, "/bin/%s", args2[0]);
						if(!isCommand(caminho2)){
							printf("Comando %s nao valido\n", args2[0]);
						}else{
							if(redirecionamento2 == NULL){
								args2 = (char**)realloc( args2, (args2size + 1) * sizeof(char*) );
								args2[argsSize+1] = NULL;

								switch( pid = fork() ){
							        case -1: 
							            perror("Can't fork");
							            exit(1);
							        case 0:
							        	close(cp[1]);
							        	dup2(cp[0],0);
										execvp(caminho2, args2);
					        			perror("No exec");
					            		exit(1);

					            	default:
					            		
					            		if(background == NULL){
					            			waitpid(pid, NULL, WCONTINUED);
					            		}
					            		close(cp[0]);
								}
							}else if(redirecionamento2 != NULL){


								if(strcmp(redirecionamento2, "=>")==0){
						
									for (int i = 0; i < args2size; ++i){
										if(!strcmp( args2[i], "=>" )){
											file2 = (char*)calloc( (strlen(args2[i+1])) , sizeof(char) );
											strcpy( file2, args2[i+1] );
											args2[i] = NULL;
											break;
										}
									}

									
								    switch( pid = fork() ){
								        case -1: 
								            perror("Can't fork");
								            exit(1);
								        case 0:

								        	close(cp[1]);
								        	close(cp2[0]);
								        	
								        	dup2(cp2[1],1);
								        	dup2(cp[0],0);

								        	execvp(caminho2, args2);
						        			perror("No exec");
						            		exit(1);

						            	default:
						            		
						            		waitpid(pid, NULL, WCONTINUED);
											close(cp2[1]);
									}
									
									
							    	switch(pid = fork()) {
								        case -1: 
								            perror("Can't fork");
								            exit(1);
								        case 0: 
											//define o caminho para o arquivo de saida
											close(cp2[1]);
											close(cp[1]);
											close(cp[0]);
											filepath2 = (char*)calloc(1024 , sizeof(char));
											sprintf(filepath2,"/home/igor/Documents/sistemas-operacionais/%s", file2);
											
											filedesc = open(filepath2, O_CREAT | O_WRONLY, 0777);
											
											while(read(cp2[0], &ch, 1)>0) {
												write(filedesc, &ch, 1);
											}
						            		close( cp2[0]);
						            		close(filedesc);
						            		free(filepath2);
						            		exit(1);
						            	default:
						            		if(background == NULL){
						            			waitpid(pid, NULL, WCONTINUED);
						            		}
						            		
									}
									free(file2);

									for (int i = 0; i < args2size; ++i){
										if(args2[i] == NULL){
											char *bla = (char*)calloc(strlen("=>") , sizeof(char));
											strcpy(bla, "=>");
											args2[i] = bla;
											free(bla);
										}
									}
								}
							}
						}
					}				
					free(filepath);
					
				}
			}
		}
	}
		/*---------------Desaloca o alocado durante a execução----------------*/

		for (int i = 0; i < argsSize; ++i){
			
			free(args[i]);
		}
		free(args);
		free(caminho);

		if( pipeFlag == 1 ){
			for (int i = 0; i < args2size; ++i){

				free(args2[i]);
			}
			free(caminho2);
			free(args2);
		}
		/*-----------------------------------------------------------*/
	


	return 0;
}
