#include <stdlib.h>
#include <stdio.h>
#include "fila.h"
Fila *criafila(int *tamanho){
	Fila *fila = malloc(sizeof(Fila));
	fila->tamanho = *tamanho;
	fila->primeiro = 0;
	fila->ultimo = 0;
	fila->items = (Item_fila*)calloc(*tamanho, sizeof(Item_fila));
	return fila;
}

Item_fila* novo_item_fila(){
	Item_fila *item = (Item_fila*)calloc(1, sizeof(Item_fila));
	return item;
}

void insere(Fila *f, Item_fila *i){
	
	f->ultimo++;
	f->cont++;
	if(f->ultimo >= f->tamanho && f->primeiro > 0){
		f->ultimo = 0;
	}
	if(f->items == NULL) printf("nulo\n");
	f->items[f->ultimo] = *i;

}

void desenfilera(Fila *f){

	f->primeiro++;
	f->cont--;
	if(f->primeiro == f->tamanho){
		f->primeiro = 0;
	}

}

void deleta_fila(Fila *f) {

	if(f->primeiro != f->ultimo){
		for(int i = 0; i < f->ultimo ; i++){
			free(&f->items[i]);
		}
	}
	free(f);
}