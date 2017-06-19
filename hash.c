#include<stdio.h>
#include<stdlib.h>
#include"hash.h"

// Criar para apontar para o mesmo bucker

Hash* cria_Hash(){
	Hash* ha = (Hash*)malloc(sizeof(Hash));
	ha->profundidade = 1;
	ha->lista = (Buckets**)malloc(2 * sizeof(Buckets*));
	ha->lista[0] = NULL;
	ha->lista[1] = NULL;
	ha->NBuckets = 0;
	return ha;
}

int expandirHah(Hash *ha){
	if(ha == NULL)
		return 0;

	free(ha->lista);
	ha->profundidade++;
	ha->lista = (Buckets**)malloc(exponencial(2,ha->profundidade) * sizeof(Buckets*));
	return 1;
}

int criaBin(int nBits, int valorInt){

	int resultado = 0;
	int i;
	int dec = 1;
	int rest;
	int div;
	for(i = 0; i < nBits; i++){
		rest = valorInt % 2;
		resultado = resultado + (rest * dec);
		dec = dec * 10;
		valorInt = valorInt / 2;
		if(valorInt == 0)
			break;
	}
	return resultado;


}

int criaInt(int valor){

	int resultado = 0, exp = 1;
	int i,cont = -1, dec = 1;

	while(dec <= valor){
		dec *= 10;
		cont++;
	}

	dec /= 10;

	while(cont >= 0){
		for(i = 0; i < cont; i++)
			exp *= 2;
		resultado += ((valor/dec) * exp);
		valor = valor % dec;
		dec /= 10;
		cont--;
		exp = 1;
	}

	return resultado;

}

int funcaoHash1(int valor){
	return (valor & 0x7FFFFFFF);
}
int funcaoHash2(int valor, int profundidade){
	return criaInt(criaBin(profundidade,valor));
}

int insere_Hash(Hash *ha, int valor){

	if(ha == NULL)
		return 0;

	int pos = funcaoHash2(valor,ha->profundidade);

	if(ha->lista[pos] == NULL){
		Buckets *newBu = cria_Bucket(ha->profundidade-1, ha);
		ha->lista[0] = newBu;
		ha->lista[1] = newBu;
		insere_Bucket(ha->lista[pos], valor);
		return 1;
	}

	if(ha->lista[pos]->qtd == 4){

		if(ha->lista[pos]->profundidadeLocal < ha->profundidade){
			//multiplas conexões

			int NConexoes = exponencial(2,(ha->profundidade - ha->lista[pos]->profundidadeLocal));
			// guardar em um vetor os valores do bucket atual
			// limpar bucket atual
			// criar novo bucket nas novas posições
			// armazenar valores do vetor
			// armazenar o novo valor

			int temporario[4],i;

			for(i = 0; i < 4; i++)
				temporario[i] = ha->lista[pos]->valor[i];

			limpar_bucket(ha->lista[pos]);

			int primeiro = criaInt(criaBin(ha->lista[pos]->profundidadeLocal, pos));

			Buckets *newBu = cria_Bucket(ha->lista[pos]->profundidadeLocal, ha);

			for(i = 0; i < NConexoes/2; i++){
				ha->lista[primeiro] = newBu;
				primeiro += NConexoes;
			}

			for(i = 0; i < 4; i++)
				insere_Hash(ha, temporario[i]);

			insere_Hash(ha, valor);

			return 1;

		}

		if(ha->lista[pos]->profundidadeLocal == ha->profundidade){
			// pegar todos os enderços
			// expandir hash hash
			// redistribuir os endereços
			// limpar bucket
			// criar novo bucket
			// substituir um dos endereçoes de bucket

			int NConexoes;

			Buckets* endBuckets[exponencial(2,ha->profundidade)];
			int i,j,salto;
			for(i = 0; i < exponencial(2,ha->profundidade); i++)
				endBuckets[i] = ha->lista[i];

			expandirHah(ha);

			for(i = 0; i < exponencial(2,ha->profundidade)/2; i++)
				ha->lista[i] = endBuckets[i];

			int count = 0, limitador = exponencial(2,ha->profundidade);

			for(i = 0; i < limitador; i++){
				NConexoes = exponencial(2,((ha->profundidade - 1) - ha->lista[i]->profundidadeLocal));
				Buckets *buTemp = ha->lista[i];
				salto = i;
				for(j = 0; j < NConexoes*2; j++){
					ha->lista[salto] = buTemp;
					salto += exponencial(2,ha->profundidade) / (NConexoes*2);
					count++;
				}
				if(count == exponencial(2,ha->profundidade))
					break;
			}

			int temporario[4];

			for(i = 0; i < 4; i++)
				temporario[i] = ha->lista[pos]->valor[i];

			limpar_bucket(ha->lista[pos]);

			int primeiro = criaInt(criaBin(ha->lista[pos]->profundidadeLocal, pos));

			Buckets *newBu = cria_Bucket(ha->lista[pos]->profundidadeLocal, ha);

			ha->lista[primeiro] = newBu;

			for(i = 0; i < 4; i++)
				insere_Hash(ha, temporario[i]);

			insere_Hash(ha, valor);

			return 1;

		}


	}

	insere_Bucket(ha->lista[pos], valor);

	return 1;

}

Buckets* cria_Bucket(int profundidadeLocal, Hash *ha){
	Buckets *bu = (Buckets*)malloc(sizeof(Buckets));
	if(bu != NULL){
		ha->NBuckets++;
		bu->NBucket = ha->NBuckets;
		bu->profundidadeLocal = profundidadeLocal;
		bu->qtd = 0;
		return bu;
	}
}

int insere_Bucket(Buckets *bu, int valor){
	if(bu == NULL)
		return 0;
	bu->valor[bu->qtd] = valor;
	bu->qtd++;
	return 1;
}

int exponencial(int varivel, int expoente){
	int i, resultado = 1;

	for (i = 0; i < expoente; ++i)
	{
		resultado *= varivel;
	}

	return resultado;
}

int limpar_bucket(Buckets *bu){
	if(bu == NULL)
		return 0;

	bu->qtd = 0;
	bu->profundidadeLocal++;

	return 1;
}

int bitsPedidos(int valorBi, int nBits){

	int i,resultado = 0, cont = 0, dec = 1;

	while(dec <= valorBi){
		dec *= 10;
		cont++;
	}

	dec /= 10;

	while(cont != 0){
		if(cont <= nBits){
			resultado += dec;
		}
		valorBi = valorBi % dec;
		dec /= 10;
		cont--;
	}

	return resultado;

}

int imprimiValores(Hash *ha){
	if(ha == NULL)
		return 0;

	// quantos campor tem a hash

	int i,j, count = exponencial(2,ha->profundidade);
	int Nbu = ha->NBuckets;
	int vetorTeste[Nbu];

	for(i = 0; (i < count) && (Nbu != 0); i++){
		if(vetorTeste[ha->lista[i]->NBucket-1] != ha->lista[i]->NBucket){
			for(j = 0; j < ha->lista[i]->qtd; j++){
				printf("%dª valor do Bucket %d: %d.\n",j+1,ha->lista[i]->NBucket,ha->lista[i]->valor[j]);
			}
			vetorTeste[ha->lista[i]->NBucket-1] = ha->lista[i]->NBucket;
			Nbu--;
		}
	}

	return 1;
}

int buscaValores(Hash *ha, int valor){
	if(ha == NULL)
		return 0;

	int i, count = exponencial(2,ha->profundidade);
	int Nbu = ha->NBuckets;
	int vetorTeste[Nbu];

	int pos = criaInt(criaBin(ha->profundidade,valor));

	for(i = 0; i < ha->lista[pos]->qtd; i++){
		if(ha->lista[pos]->valor[i] == valor)
			return ha->lista[pos]->NBucket;
	}

	return -1;

}

int remocao_Elemento(Hash *ha, int valor){
	if(ha == NULL)
		return 0;

	int i,j, count = exponencial(2,ha->profundidade);
	int Nbu = ha->NBuckets;
	int vetorTeste[Nbu];

	int pos = criaInt(criaBin(ha->profundidade,valor));

	if(ha->lista[pos]->qtd == 0)
		return 0;

	printf("pos: %d.\n",pos);

	for(i = 0; i < ha->lista[pos]->qtd; i++){
		if(ha->lista[pos]->valor[i] == valor){
			for(j = i; j < ha->lista[pos]->qtd-1; j++){
				ha->lista[pos]->valor[j] = ha->lista[pos]->valor[j+1];
			}
			ha->lista[pos]->qtd--;
			return 1; // tinha o elemento
		}
	}

	return 0; // não tinha o elemento
}
