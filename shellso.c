#include "shellso.h"	

int main(int argc, char **argv[]){
	//variaveis
	char *entrada = (char*)malloc(1024 * sizeof(char));
	char *copiaSegura = (char*)malloc(1024 * sizeof(char));
	char **args;
	char **redi;
	int argssize;
	int pid;
    int pc[2]; /* Parent to child pipe */
    int cp[2]; /* Child to parent pipe */
    char ch;
    int incount = 0, outcount = 0;
	int status;
	int redirect;
	
	FILE *arq;

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
		fgets(entrada, 1024, stdin); //pega a entrada do terminal
		strcpy(copiaSegura, entrada);
		if(!strcmp(copiaSegura, "quit\n")) break;
		//caso o usuario digite enter o loop salta pra proxima iteracao
		if(!strcmp(copiaSegura, "\n")) continue;
		//remove o \n da string
		if(entrada[strlen(entrada)-1] == '\n'){ 
			entrada[strlen(entrada)-1] = '\0';
		}
		//tokeniza a entrada do usuario
		
		char **comandoComArgumentos;
		char *redirecionamento; 
		char *arquivo;
		char *comando;
		args = tokeniza(entrada, &argssize);
		int numDeArgumentos = verificaSimbolos(args, argssize);
		
		if(!isRedirectCorrect(args, &argssize)){
			printf("Nao e possivel utilizar este simbolo para redirecionar a saida do programa.\n");
			continue;
		}
		
		
		comando = getComando(args);
		comandoComArgumentos = getArgumentos(args, argssize);
		redirecionamento = getRedirecionamento(args, argssize);
		arquivo = getArquivo(args, argssize);
		//printf("%d\n", numDeArgumentos);
		//printf("comando: %s\n", comando);
		/*for(int i = 0; i< argssize  ;i++ ){
			printf("argumento %d: %s\n",i, comandoComArgumentos[i] );
		}/*
		printf("redirecionamento: %s\n", redirecionamento);
		printf("arquivo: %s\n", arquivo);*/

		//cocatena o caminho /usr/bin/ ao comando digitado
		char *caminho = (char*)malloc(1024*sizeof(char));
		sprintf(caminho, "/bin/%s", comando);
		
		


		//verifica se o comando e valido
		if(!isCommand(caminho)){
			printf("Comando %s nao valido\n", entrada);
		}
		//se valido realiza o fork
		else{
			
			
			if(redirecionamento == NULL){
				
				switch( pid = fork() ){
			        case -1: 
			            perror("Can't fork");
			            exit(1);
			        case 0:
			        	execvp(caminho, comandoComArgumentos);
	        			perror("No exec");
	            		exit(1);

	            	default:
	            		
	            		waitpid(pid, &status, WCONTINUED);
				}
			}if(redirecionamento != NULL) {
				char *filepath = (char*)malloc(1024 * sizeof(char));
				char *buffer = (char*)malloc(1024*sizeof(char));
				if(strcmp(redirecionamento, "=>")==0){
					

				/* Create a child to run command. 
			    switch( pid = fork() ){
			        case -1: 
			            perror("Can't fork");
			            exit(1);
			        case 0:

			        	dup2(cp[1], 1);

			        	
			        	close(pc[0]);/* Make stdin come from read
	                            end of pipe. 
			        	close( pc[1]);
	            		close( cp[0]);
	            		execvp(caminho, comandoComArgumentos);
	        			perror("No exec");
	            		exit(1);

	            	default:
	            		
	            		waitpid(pid, &status, WCONTINUED);
						close(cp[1]);
				}
				
				
		    	switch(pid = fork()) {
			        case -1: 
			            perror("Can't fork");
			            exit(1);
			        case 0: 
						//define o caminho para o arquivo de saida
						close(cp[1]);
						close(pc[0]);
						sprintf(filepath,"/home/igor/Documents/sistemas-operacionais/%s", arquivo);
						
						arq = fopen(arquivo,"w");
						fclose(arq);
			       		int filedesc = open(filepath, O_WRONLY);
						
						while(read(cp[0], &ch, 1)>0) {
							
							write(filedesc, &ch, 1);
						}
			        	close( pc[1]);
	            		close( cp[0]);
	            		exit(1);
	            	default:
	            		waitpid(pid, &status, WCONTINUED);	
				}   */
				}else
				if(strcmp(redirecionamento, "<=")==0){
					switch(pid = fork()) {
				        case -1: 
				            perror("Can't fork");
				            exit(1);
				        case 0:
				        	sprintf(filepath, "/home/igor/Documents/sistemas-operacionais/%s", arquivo);
							if(!isCommand(filepath)){
								printf("Comando %s nao valido\n", entrada);
							}
							int filedesc = open(filepath, O_RDONLY);
							dup2(filedesc,0);
							dup2(cp[1],1);
							while(read(filedesc, &ch, 1)){
								write(cp[1], &ch, 1);
							}
							close(cp[0]);
							close(pc[0]);
							close(pc[1]);

							exit(1);
							
							
		            	default:

		            		waitpid(pid, &status, WCONTINUED);
							close(cp[1]);
					}

					switch(pid = fork()) {
				        case -1: 
				            perror("Can't fork");
				            exit(1);
				        case 0:
				        	
							
							dup2(cp[0],0);
							
							read(cp[0], buffer, 1024);
							comandoComArgumentos[numDeArgumentos] = buffer;
							comandoComArgumentos[numDeArgumentos+1] = NULL;
							
							execvp(caminho, comandoComArgumentos);
							perror("No exec");
							close(cp[0]);
							close(pc[0]);
							close(pc[1]);

							exit(1);
							
							
		            	default:

		            		waitpid(pid, &status, WCONTINUED);
					}

				}
       		}
		}		
	}
	return 0;
}
