#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h> 
#include <math.h>

#include "struct.h"
#include "tabelahash.h"
#include "contador.h"

#define MAX_WORD_SIZE 100  

char * getNextWord(FILE * fp, bool * state) {
	char c;
	int i = 0;
	char * word = (char *) malloc( sizeof(char) * MAX_WORD_SIZE );
	while ( ( (c = fgetc(fp)) != EOF ) ) {
		// FIXME : Talvez seja melhor só tirar essas verificações de '.,?!', e verificar o quanto isso afeta performance.
		if (c == ' ' || c == '\n' || c == '.' || c == ',' || c == '?' || c == '!' || c == '\r' || c == '\t' ) {
			// Letras de saída
			if (*state == true) { 
				// Se estiver no meio de uma palavra, e encontrou uma letra de saída :

				if (i == 0) {
					// Garante que não vai dar erros caso o arquivo comece com espaço.
					continue;
				}

				*state = false;
				word[i+1] = '\0';
				return word;
			}
			
		} else {
			// Letras de entrada
			if (c == '\'' || c == '\"' || c == '*' ) {
				// Reconhece a letra, porem ignora e continua.
				continue;
			}

			word[i] = c;
			i++;
			if (*state == false) {
				*state = true;
			}

			
		}
	}
	// File ended.
	if (i > 0)
		return word;
	return NULL;
}

node** getAllElementsFromHashTable(hashTable* table) {
	node **list;
	if ( (list = malloc( sizeof(node *) * table->tamanho_atual )) == NULL )
		return NULL;

	int i = 0;
	int j = 0;
	node * nodo = NULL;
	while (i < table->tamanho) {
		nodo = table->list[i];
		i++;


		if (nodo == NULL) {
			continue;
		}


		// Found a node, follow this sequence.
		while (nodo != NULL) {
			list[j] = nodo;
			j++;
			//printf("%s : %d\n", nodo->key, nodo->value);
			nodo = nodo->next;
		}
	}

	return list;
}

int node_comp(const void* p1, const void* p2) {
	int l = (*((node **)p1))->value;
	int r = (*((node **)p2))->value;
	return r - l;
}

int doc_rel_comp(const void* p1, const void* p2) {
	double l = (*((documentoRelevancia **)p1))->relevancia;
	double r = (*((documentoRelevancia **)p2))->relevancia;
	if (l > r)
		return -1;
	else
		return 1;
}

termosList* createTermosList(char* value) {
	termosList * novoString = NULL;

	// Garante que a alocação foi feita sem erros.
	if ( ( novoString = malloc(sizeof(termosList)) ) == NULL )
		return NULL;

	// Garante que a atribuição de chave foi feita sem erros.
	if ( ( novoString->value = strdup(value) ) == NULL )
		return NULL;

	novoString->nDocs = 0;
	novoString->next = NULL;
	return novoString;
}

arquivosList* createArquivosList(char* value) {
	arquivosList * novoString = NULL;

	// Garante que a alocação foi feita sem erros.
	if ( ( novoString = malloc(sizeof(arquivosList)) ) == NULL )
		return NULL;

	// Garante que a alocação foi feita sem erros.
	if ( ( novoString->table = malloc(sizeof(hashTable)) ) == NULL )
		return NULL;

	// Garante que a atribuição de chave foi feita sem erros.
	if ( ( novoString->value = strdup(value) ) == NULL )
		return NULL;

	novoString->countPalavras = 0;
	novoString->next = NULL;
	return novoString;
}

documentoRelevancia* createDocumentoRelevancia(char* value, double relevancia) {
	documentoRelevancia* novoDocRel = NULL;

	// Garante que a alocação foi feita sem erros.
	if ( ( novoDocRel = malloc(sizeof(documentoRelevancia)) ) == NULL )
		return NULL;

	// Garante que a atribuição de chave foi feita sem erros.
	if ( ( novoDocRel->value = strdup(value) ) == NULL )
		return NULL;

	novoDocRel->relevancia = relevancia;
	return novoDocRel;

}

int calcFreq (FILE * fp, hashTable * table) {
	bool state = true; // E se o arquivo de texto começar com espaço? Já resolvido dentro da função getNextWord.
	int count = 0;
	char * palavra;
	while ( (palavra = getNextWord(fp, &state)) != NULL ) {
		incrementHashTable(table, palavra);
		free(palavra);
		count++;
	}
	return count;
}