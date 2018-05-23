typedef struct{
	unsigned *pagina_referenciada;
	int *ultimo_acesso;
	int *pagina_alterada;
}Entrada;

typedef struct {
    unsigned *key;
    Entrada* value;
    struct Item *next;
}Item;

typedef struct {
    int tamanho;
    int cont;
    Item **items;
}Tabela_hash;

Item* novo_item(unsigned *k, Entrada *v);
Tabela_hash* nova_tabela_hash(int *tamanho);
void deleta_item(Item *i);
void deleta_tabela(Tabela_hash *th);