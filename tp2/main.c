#include "main.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "tabela_hash.h"
#include "fila.h"
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
	if(tamanhoDePagina >=2 && tamanhoDePagina <= 64){
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
	int numeroDeAcertos = 0;
	int numeroDeFalhas = 0;
	int numeroDePaginasSujas = 0;
	int tempo = 0;


	unsigned s, tmp;
	int addr_shiftado;
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
	Tabela_hash *memoriaVirtual = nova_tabela_hash(&num_de_paginas);

	Fila *mem_principal = criafila(&tamanhoDaMemoriaFisica);

	int index;

	int cont = 0;
	unsigned x;
	int flagAcerto = 0;
	if( !strcmp( algoritmo, "lru" ) ){
		while(!feof(file)){
			fscanf(file,"%x %c", &addr, &rw);
			//printf("%x %c\n", addr, rw);
			addr_shiftado = addr >> s;
			index = addr_shiftado%memoriaVirtual->tamanho;
			flagAcerto = 0;

			if(memoriaVirtual->items[index]->next == NULL){

					numeroDeFalhas++;

					Item_fila *i = novo_item_fila();
					i->offset = addr_shiftado;
					insere(mem_principal, i);

					Entrada *e = (Entrada*)calloc(1, sizeof(Entrada));

					e->pagina_referenciada = i->offset;
					e->ultimo_acesso = tempo;
					e->pagina_alterada = 0;
					e->presente_ausente = 1;

					memoriaVirtual->items[index]->next = novo_item(&addr_shiftado, e);


			}else{
				Item *p = memoriaVirtual->items[index];
				for( ; p->next != NULL ; p = p->next){
					if(p->next->value->pagina_referenciada == addr_shiftado &&
					 p->next->value->presente_ausente == 1 ){
					 	flagAcerto = 1;
						numeroDeAcertos++;

					}else
					if(p->next->value->pagina_referenciada == addr_shiftado &&
				 	 p->next->value->presente_ausente == 0 ){
					 	p->next->value->ultimo_acesso = tempo;
						p->next->value->pagina_alterada = 1;
						p->next->value->presente_ausente = 1;
						int indexx = mem_principal->items[mem_principal->primeiro].offset;
						indexx = indexx%memoriaVirtual->tamanho;
						for( ; p->next != NULL ; p = p->next){
							if(p->next->value->pagina_referenciada == addr_shiftado){
								p->next->value->ultimo_acesso = tempo;
								p->next->value->pagina_alterada = 1;
								p->next->value->presente_ausente = 0;
							}
						}

						desenfilera(mem_principal);
						numeroDeFalhas++;
						numeroDePaginasSujas++;
						Item_fila *i = novo_item_fila();
						i->offset = addr;
						insere(mem_principal, i);
					}else{
						continue;
					}
				}

				if(flagAcerto != 1){
					if (mem_principal->cont == mem_principal->tamanho){

						int indexx = mem_principal->items[mem_principal->primeiro].offset;
						indexx = indexx%memoriaVirtual->tamanho;
						for( ; p->next != NULL ; p = p->next){
							if(p->next->value->pagina_referenciada == addr_shiftado){
								p->next->value->ultimo_acesso = tempo;
								p->next->value->pagina_alterada = 1;
								p->next->value->presente_ausente = 0;
							}
						}

						desenfilera(mem_principal);
					}
					numeroDeFalhas++;
					Item_fila *i = novo_item_fila();
					i->offset = addr;
					insere(mem_principal, i);

					Entrada *e = (Entrada*)calloc(1, sizeof(Entrada));

					e->pagina_referenciada = i->offset;
					e->ultimo_acesso = tempo;
					e->pagina_alterada = 0;
					e->presente_ausente = 1;

					p->next = novo_item(&addr_shiftado, e);

				}
			}
			//printf("Acertos => %d\n", numeroDeAcertos);
			//printf("Falhas => %d\n\n", numeroDeFalhas);
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
	printf("cont mem mem_principal: %d\n",cont);
	printf("Numero de acertos: %d\n", numeroDeAcertos);
	printf("Numero de falhas: %d\n", numeroDeFalhas);
	printf("Numero de paginas sujas: %d\n", numeroDePaginasSujas);

//=========================================================================//

	return 0;
}
