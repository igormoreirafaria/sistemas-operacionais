#include "shellso.h"



int main(int argc, char **argv[]){
	char *comando = (char*)malloc(1024 * sizeof(char));
	char *copiaSegura = (char*)malloc(1024 * sizeof(char));
	char **args;
	int argssize;
	int pid;
    int pc[2]; /* Parent to child pipe */
    int cp[2]; /* Child to parent pipe */
    char ch;
    int incount = 0, outcount = 0;
	int status;
	int redirect;


    /* Make pipes */
    if( pipe(pc) < 0){
        perror("Can't make pipe");
        exit(1);
    }
    if( pipe(cp) < 0){
        perror("Can't make pipe");
        exit(1);
    }

	printf("Digite seu comando ou digite quit pra sair\n");

	while(true){
		 //imprime $ na tela
		printf(SHELLMARK);
		//pega a entrada do terminal
		fgets(comando, 1024, stdin); //pega a entrada do terminal
		strcpy(copiaSegura, comando);
		if(!strcmp(copiaSegura, "quit\n")) break;
		//caso o usuario digite enter o loop salta pra proxima iteracao
		if(!strcmp(copiaSegura, "\n")) continue;
		//remove o \n da string
		if(comando[strlen(comando)-1] == '\n'){ 
			comando[strlen(comando)-1] = '\0';
		}

		//tokeniza a entrada do usuario
		args = tokeniza(comando, args, &argssize);
		
		if(!isRedirectCorrect(args, &argssize)){
			printf("Nao e possivel utilizar este simbolo para redirecionar a saida do programa.\n");
			continue;
		}

		//cocatena o caminho /usr/bin/ ao comando digitado
		char *caminho = (char*)malloc(1024*sizeof(char));
		sprintf(caminho, "/bin/%s", args[0]);
		
		redirect = isRedirect(args, &argssize);

		//verifica se o comando e valido
		if(!isCommand(caminho)){
			printf("Comando %s nao valido\n", args[0]);
		}
		//se valido faz o fork
		else{
			/* Create a child to run command. */
		    switch( pid = fork() ){
		        case -1: 
		            perror("Can't fork");
		            exit(1);
		        case 0:
		        	dup2(1, cp[1]);/* Make stdout go to write
                            end of pipe. */
		        	dup2(0, pc[0]);/* Make stdin come from read
                            end of pipe. */
		        	close( pc[1]);
            		close( cp[0]);
            		execvp(caminho, args);
        			perror("No exec");
            		exit(1);

            	default:
            		close(pc[0]);
            		close(cp[1]);
            		while(read(cp[0], &ch, 1) == 1){
            			write(1,&ch, 1);
            		}
            		waitpid(pid, &status, WCONTINUED);
			}

			

			if(redirect == 2){
		    	switch( pid = fork() ){
			        case -1: 
			            perror("Can't fork");
			            exit(1);
			        case 0:
			        	dup2(1, cp[1]);/* Make stdout go to write
	                            end of pipe. */
			        	dup2(0, pc[0]);/* Make stdin come from read
	                            end of pipe. */	

			       		FILE *arq = fopen(args[2], "w");

			       		while(read(cp[0], &ch, 1) == 1){

			       			fwrite(&ch,1,1,arq);
			       		}


			        	close( pc[1]);
	            		close( cp[0]);
	            		execvp(caminho, args);
	        			perror("No exec");
	            		exit(1);

	            	default:

	            		waitpid(pid, &status, WCONTINUED);
	            		
				}   			
       		}


		}		
	}

	return 0;
}