typedef struct{
		unsigned offset;
}Item_fila;

typedef struct{
	int tamanho;
	int cont;
	int primeiro;
	int ultimo;
	Item_fila *items;

}Fila;

Fila *criafila(int *tamanho);
Item_fila *novo_item_fila();
void insere(Fila *f, Item_fila *i);
void desenfilera(Fila *f);
void deleta_fila(Fila *f);
