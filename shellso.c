#include "shellso.h"	



int main(int argc, char **argv[]){
	char *comando = (char*)malloc(1024 * sizeof(char));
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
	args = (char**)malloc(3*sizeof(char*));

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
		//tokeniza(comando, args, redi, &argssize);
				
		char *argumento;
		char *redirecionamento; 
		char *arquivo;
		char *fcomando;


		int i = 0,tamanho;
	    char *palavra;
	    char *ncomando;
	    

	    tamanho = strlen(comando);

	    ncomando = (char*)malloc(tamanho*sizeof(char));
	    int tamcm = 0;
	    char *com; 
	    strcpy(ncomando,comando);
	    
	    palavra = strtok(comando," ");
	       
	    while( palavra !=  NULL ) {
	      palavra  =  strtok(NULL," ");
	      tamcm++;
	    }
	    
	    lista_comando* lista;
	    lista = (lista_comando*)malloc(tamcm*sizeof(lista_comando));
	    if(lista == NULL){
	        printf("Erro\n");
	        exit(1);
	    }

	    char *pal;
	    int n = 0; 
	    
	    pal = strtok(ncomando," ");
	    while( pal !=  NULL ) {
			strcpy(lista[n].palavras,pal);
			pal  =  strtok(NULL," ");
			n++;
	    }  
	    int j = 0,k = 0,r = 0,o = 0,confirma = -1;
	    for(i = 0;i<tamcm;i++){
	       
	       k = strlen(lista[i].palavras);
	       
	     
	       for(j = 0;j<k;j++){
	           o = lista[i].palavras[j];
	           if(o == 47){//Verica argumento do comando
	                
	                r = strlen(lista[i].palavras);
	                argumento = (char*)malloc(r*sizeof(char));
	                strcpy(argumento,lista[i].palavras);
	           }
	           if(o == 60 || o == 62){//Verifica redirecionamento
	                //printf("Redirecionamento  =  %s \n",lista[i].palavras);
	                r = strlen(lista[i].palavras);
	                redirecionamento = (char*)malloc(r*sizeof(char));
	                strcpy(redirecionamento,lista[i].palavras);
					
	                confirma = 1; 
	           }        
	       }      
	    }
	   
	    fcomando = (char*)malloc(strlen(lista[0].palavras)*sizeof(char));
	    strcpy(fcomando,lista[0].palavras);
	   	
	    if(confirma == 1){
	        k = strlen(lista[tamcm-1].palavras);
	        
	        arquivo = (char*)malloc (k*sizeof(char));
	        strcpy(arquivo,lista[tamcm-1].palavras);
	    }

		/*if(!isRedirectCorrect(args, &argssize)){
			printf("Nao e possivel utilizar este simbolo para redirecionar a saida do programa.\n");
			continue;
		}*/

		//cocatena o caminho /usr/bin/ ao comando digitado
		char *caminho = (char*)malloc(1024*sizeof(char));
		sprintf(caminho, "/bin/%s", fcomando);
		args[0] = fcomando;
		args[1] = argumento;

		//verifica se o comando e valido
		if(!isCommand(caminho)){
			printf("Comando %s nao valido\n", comando);
		}
		//se valido faz o fork
		else{
			/* Create a child to run command. */
		    switch( pid = fork() ){
		        case -1: 
		            perror("Can't fork");
		            exit(1);
		        case 0:
		        	dup2(cp[1], 1);/* Make stdout go to write
                            end of pipe. */
		        	dup2(pc[0], 0);/* Make stdin come from read
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

			

			if(redirecionamento != NULL){
		    	switch( pid = fork() ){
			        case -1: 
			            perror("Can't fork");
			            exit(1);
			        case 0:
			        	
			        	dup2(cp[1], 1);
			        	
			        	dup2(pc[0], 0); 

			       		FILE *arq = fopen(arquivo, "w");
			       		char *str;
			       		while(read(cp[0], &ch, 1) == 1){

			       			write(*str,&ch,1);
			       			fwrite(str , 1 , sizeof(str) , arq );
			       		}

			        	close( pc[1]);
	            		close( cp[0]);
	            		
	            		exit(1);

	            	default:

	            		waitpid(pid, &status, WCONTINUED);
	            		
				}   			
       		}


		}		
	}

	return 0;
}