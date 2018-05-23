#include "main.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "tabela_hash.h"
int main(int argc, char const *argv[]){

	if(argc < 5){
		printf("Erro. Numero de argumentos insuficientes\n");
		exit(1);
	}

	//imprime argv
	// for (int i = 1; i < argc; ++i){
	// 	printf("%s\n", argv[i]);
	// }
	
//=========================tratamento da entrada============================//
	char *algoritmo = ( char* )calloc( strlen(argv[1]),  sizeof( char* ) );
	strcpy( algoritmo, argv[1] );
	//printf("%s\n", algoritmo);

	char *nomeDoArquivo = ( char* )calloc( strlen(argv[2]),  sizeof( char* ) );
	strcpy( nomeDoArquivo, argv[2] );
	char *caminhoDoArquivo = ( char* )calloc( 1024,  sizeof( char* ) );
	sprintf(caminhoDoArquivo, "entradas/%s",nomeDoArquivo);
	//printf("%s\n", nomeDoArquivo);

	int tamanhoDePagina = atoi( argv[3] );
	if(tamanhoDePagina >=2 && tamanhoDePagina <= 128){
		tamanhoDePagina = tamanhoDePagina * 1024;
		//printf("%d\n", tamanhoDePagina);
	}else{
		printf("Erro: O tamanho da pagina deve ser um valor entre 2 e 64.\n");
		return 0;
	}

	int tamanhoDaMemoriaFisica = atoi( argv[4] );
	if(tamanhoDaMemoriaFisica >= 128 && tamanhoDaMemoriaFisica <= 16384 ){
		tamanhoDaMemoriaFisica = tamanhoDaMemoriaFisica * 1024;
		//printf("%d\n", tamanhoDaMemoriaFisica);
	}else{
		printf("Erro: O tamanho da memoria fisica deve ser um valor entre 128 e 16384.\n");
		return 0;
	}
//==========================================================================//
//==============================execucao====================================//
	int numeroDeAcessos = 0;
	int numeroDeFalhas = 0;
	int numeroDePaginasSujas = 0;
	int tempo = 0;


	unsigned s, tmp;
	int page;
	/* Derivar o valor de s: */
	tmp = tamanhoDePagina;
	s = 0;
	while (tmp>1) {
		tmp = tmp>>1;
		s++;
	}

	unsigned addr;
	char rw;
	FILE *file = fopen(caminhoDoArquivo, "r");
	
	int num_de_paginas = tamanhoDaMemoriaFisica/tamanhoDePagina;
	int index;
	int tam_mem_v = pow(num_de_paginas,2);
	Tabela_hash *memoriaVitual = nova_tabela_hash(&tam_mem_v);

	if( !strcmp( algoritmo, "lru" ) ){
		while(!feof(file)){
			fscanf(file,"%x %c", &addr, &rw);
			//printf("%x %c\n", addr, rw);
			page = addr >> s;
			//index = page%num_de_paginas;

			
			
			

			//printf("%x\n", page);
			tempo++;
		}
	}else if( !strcmp( algoritmo, "nru" ) ){

	}else if( !strcmp( algoritmo, "segunda_chance" ) ){

	}

	// while(!feof(file)){
	// 	fscanf(file,"%x %c", &addr, &rw);
	// 	//printf("%x %c\n", addr, rw);
	// 	page = addr >> s;
	// 	//printf("%x\n", page);
	// 	tempo++;
	// }


//==========================================================================//
//===============================Saida=====================================//
	printf("=======================Configuracao============================\n");
	printf("Algoritmo utilizado: %s\n", algoritmo );
	printf("Arquivo de leitura utilizado: %s\n", nomeDoArquivo );
	printf("Tamanho da pagina utilizado: %d\n", ( tamanhoDePagina/1024 ) );
	printf("Tamanho da memoria fisica utilizado: %d\n",  ( tamanhoDaMemoriaFisica/1024 ) );
	printf("===============================================================\n");
	printf("\n");
	printf("Numero de acessos: %d\n", numeroDeAcessos);
	printf("Numero de falhas: %d\n", numeroDeFalhas);
	printf("Numero de paginas sujas: %d\n", numeroDePaginasSujas);

//=========================================================================//

	return 0;
}