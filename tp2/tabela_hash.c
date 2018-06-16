#include <stdlib.h>
#include <string.h>

#include "tabela_hash.h"

Item* novo_item(unsigned *k, Entrada *v) {
    Item* i = malloc(sizeof(Item));
    i->key = k;
    i->value = v;
    return i;
}

Tabela_hash* nova_tabela_hash(int *tamanho) {
    Tabela_hash* th = malloc(sizeof(Tabela_hash));

    th->tamanho = *tamanho;
    th->cont = 0;
    th->items = calloc(th->tamanho , sizeof(Item*));
    for(int i = 0; i < *tamanho ; i++){
    	th->items[i] = (Item*)calloc(1, sizeof(Item));
    }
    return th;
}

void deleta_item(Item *i) {
    free(i->key);
    free(i->value);
    free(i);
}


void deleta_tabela(Tabela_hash *th) {
    for (int i = 0; i < th->tamanho; i++) {
        Item *item = th->items[i];
        if (item != NULL) {
            deleta_item(item);
        }
    }
    free(th->items);
    free(th);
}