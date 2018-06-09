typedef struct{
	unsigned pagina_referenciada; //offset
	int ultimo_acesso;
	int pagina_alterada;
	int presente_ausente; //presente = 1 ausente = 0
}Entrada;

typedef struct item{
    int *key;
    Entrada* value;
    struct item  *next;
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