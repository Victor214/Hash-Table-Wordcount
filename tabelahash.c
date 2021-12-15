#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "struct.h"
#include "tabelahash.h"

hashTable* createHashTable (int tamanho) {

	// Se o tamanho dado for inválido
	if (tamanho < 1)
		return NULL;

	hashTable * table = NULL;

	// Aloca a Tabela Hash
	if ( (table = malloc(sizeof(hashTable)) ) == NULL )
		return NULL;

	// Aloca os ponteiros para a lista encadeada.
	if ( (table->list = malloc(sizeof( node * )*tamanho) ) == NULL )
		return NULL;

	// Garante que todos os primeiros elementos de cada uma das listas são nulos, para comparação futura.
	int i;
	for( i = 0; i < tamanho; i++ ) {
		table->list[i] = NULL;
	}

	table->tamanho_atual = 0;
	table->tamanho = tamanho;
	return table;
}

int hashKey(char * key, int tamanho) {
	unsigned long hash = 5381;
    int c;

    while (c = *key++)
        hash = ((hash << 5) + hash) + c;

    return hash%tamanho;
}

node * createNode( char * key ) {

	node * novoNodo = NULL;

	// Garante que a alocação foi feita sem erros.
	if ( ( novoNodo = malloc(sizeof(node)) ) == NULL )
		return NULL;

	// Garante que a atribuição de chave foi feita sem erros.
	if ( ( novoNodo->key = strdup(key) ) == NULL )
		return NULL;

	novoNodo->next = NULL;
	novoNodo->value = 1;
	return novoNodo;

}

node * searchHashTable( hashTable* table, char * key ) {

	// Se a tabela não existir.
	if (table == NULL)
		return NULL;

	// Retorna a posição disponível na tabela.
	int hashPos;
	hashPos = hashKey(key, table->tamanho);

	// Valor inicial da lista não é nulo, logo buscamos se já existe algo.
	node *nodoBusca = table->list[hashPos];
	node *resultante = NULL;
	while (nodoBusca != NULL) {
		// Valor encontrado
		if (strcmp(nodoBusca->key, key) == 0) {
			resultante = nodoBusca;
			break;
		}
		nodoBusca = nodoBusca->next;
	}

	return resultante;
}

void incrementHashTable( hashTable* table, char * key ) {

	// Se a tabela não existir.
	if (table == NULL)
		return;

	// Retorna a posição disponível na tabela.
	int hashPos;
	hashPos = hashKey(key, table->tamanho);

	// Valor inicial da lista não é nulo, logo buscamos se já existe algo.
	node * ultimo = NULL;
	node * nodoBusca = table->list[hashPos];
	node * resultante = NULL;
	while (nodoBusca != NULL) {
		// Valor encontrado
		if (strcmp(nodoBusca->key, key) == 0) {
			resultante = nodoBusca;
			break;
		}
		ultimo = nodoBusca;
		nodoBusca = nodoBusca->next;
	}

	if (resultante != NULL) {
		// Foi encontrado um nodo, logo basta somar um no valor do nodo.
		(resultante->value)++;
		return;
	} else {

		// Não foi encontrado nenhum valor para resultante, logo, devemos criar um novo.
		resultante = createNode(key); // Não precisa dar nenhum valor, como o valor inicial é sempre 1.
		(table->tamanho_atual)++;

		// Para quem damos esse nodo?
		if (table->list[hashPos] == NULL) {
			table->list[hashPos] = resultante;
		} else {
			// Ultimo obrigatoriamente deve existir aqui, pois entrou no while pelo menos 1 vez.
			ultimo->next = resultante;
		}
		return;
	}
}