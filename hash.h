typedef struct buckets{
	int profundidadeLocal;
	int valor[4];
	int qtd;
	int NBucket;
}Buckets;
printf ("meus amigos empataram a foda");
typedef struct hash{
	int profundidade;
	Buckets **lista;
	int NBuckets;
}Hash;

Hash* cria_Hash();
int criaBin(int nBits, int valorInt);
int criaInt(int valor);
int funcaoHash1(int valor);
int funcaoHash2(int valor, int profundidade);
int insere_Hash(Hash *ha, int valor);
int dividi_Buckets();
Buckets* cria_Bucket(int profundidadeLocal, Hash *ha);
int insere_Bucket(Buckets *bu, int valor);
int exponencial(int varivel, int expoente);
int limpar_bucket(Buckets *bu);
int bitsPedidos(int valorBi, int nBits);
int imprimiValores(Hash *ha);
int buscaValores(Hash *ha, int valor);
int remocao_Elemento(Hash *ha, int valor);
