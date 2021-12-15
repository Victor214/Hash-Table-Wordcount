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

int main( int argc, char **argv ) {
	if (argc < 2) {
		printf("Número de argumentos inválido\n");
		return 0;
	}

	if ( strcmp(argv[1], "--freq") == 0 ) {
		if (argc != 4) {
			printf("Número de argumentos inválido\n");
			return 0;
		}

		int nPalavras = atoi(argv[2]);
		char * nomeArquivo = argv[3];

		FILE * fp;
		if ( (fp = fopen(nomeArquivo, "r")) == NULL ) {
			printf("Arquivo inválido.\n");
			return 0;	
		}

		hashTable * table = createHashTable( 398341 );

		calcFreq(fp, table);

		node** lista = getAllElementsFromHashTable(table);
		qsort(lista, table->tamanho_atual, sizeof(node *), node_comp);

		// Garante que número de palavras listadas jamais passará o limite do vetor.
		int nValores = nPalavras; 
		if (nValores > table->tamanho_atual)
			nValores = table->tamanho_atual;

		for (int i = 0; i < nValores; i++) {
			printf("%s : %d\n", lista[i]->key, lista[i]->value);
		}
		printf("\n\n");

		printf("%d entradas foram listadas.\n", nValores);

	} else if ( strcmp(argv[1], "--freq-word") == 0 ) {
		if (argc != 4) {
			printf("Número de argumentos inválido\n");
			return 0;
		}

		char * palavraRef = argv[2];
		char * nomeArquivo = argv[3];

		FILE * fp;
		if ( (fp = fopen(nomeArquivo, "r")) == NULL ) {
			printf("Arquivo inválido.\n");
			return 0;	
		}

		int count = 0;
		bool state = true;
		char * palavra;
		while ( (palavra = getNextWord(fp, &state)) != NULL ) {
			if (strcmp(palavra, palavraRef) == 0)
				count++;
			free(palavra);	
		}

		printf("A palavra %s aparece %d vezes nesse documento.\n", palavraRef, count);

	} else if ( strcmp(argv[1], "--search") == 0 ) {
		if (argc < 4) {
			printf("Número de argumentos inválido\n");
			return 0;
		}

		// Salvando valor original do termo para exibir depois.
		char * termoOriginal = strdup(argv[2]);

		// Salvando parâmetros em listas ligadas.
		int totalTermos = 0;
		char *ptr = strtok(argv[2], " ");
		termosList* listaTermos = NULL;
		while (ptr != NULL) {
			termosList* string = createTermosList(ptr);
			string->next = listaTermos;
			listaTermos = string;
			ptr = strtok(NULL, " ");
			totalTermos++;
		}

		int totalDocumentos = argc-3;
		arquivosList* listaArquivos = NULL;
		for (int i = 3; i < argc; i++) {
			arquivosList* string = createArquivosList(argv[i]);
			string->next = listaArquivos;
			listaArquivos = string;
		}


		// Calculo inicial das frequências de cada arquivo em sua hash table, para uso posterior.
		// Contador de palavras totais incluso.
		termosList* stringTermos = NULL; // Será usado depois para iterar pelos termos.
		arquivosList* stringArquivos = listaArquivos;
		while (stringArquivos != NULL) {
			hashTable * table = createHashTable(398341);

			FILE * fp;
			if ( (fp = fopen(stringArquivos->value, "r")) == NULL ) {
				printf("Arquivo inválido.\n");
				return 0;	
			}

			int totalPalavras = calcFreq(fp, table);

			stringArquivos->countPalavras = totalPalavras;
			stringArquivos->table = table;

			stringArquivos = stringArquivos->next;
		}
		

		// Primeiro loop : calcula o número de documentos que X termo aparece.
		stringArquivos = listaArquivos;
		while (stringArquivos != NULL) {
			stringTermos = listaTermos;
			while (stringTermos != NULL) {
				// Se o termo estiver presente neste documento, incrementa um na qtd de documentos que o termo aparece.
				if ( searchHashTable(stringArquivos->table, stringTermos->value) != NULL )
					(stringTermos->nDocs)++;

				stringTermos = stringTermos->next;
			}


			stringArquivos = stringArquivos->next;
		}

		// Vetor para armazenar o par documentoRelevancia
		documentoRelevancia** listaDoc = malloc(sizeof(documentoRelevancia *) * totalDocumentos);


		// Segundo loop : calcula todo o resto.
		stringArquivos = listaArquivos;
		int i = 0;
		while (stringArquivos != NULL) {
			double tfidf = 0;

			stringTermos = listaTermos;
			while (stringTermos != NULL) {
				double tf = 0;
				node * tNode = searchHashTable(stringArquivos->table, stringTermos->value);
				if ( tNode != NULL )
					tf = ((float)(tNode->value))/(stringArquivos->countPalavras);

				double idf = 0;
				if (stringTermos->nDocs != 0) // Escapar divisão por 0
					idf = log10( ((float)totalDocumentos)/stringTermos->nDocs);
				
				tfidf += tf*idf;

				stringTermos = stringTermos->next;
			}

			tfidf = tfidf/totalTermos;
			documentoRelevancia * docRel = createDocumentoRelevancia(stringArquivos->value, tfidf);


			listaDoc[i] = docRel;
			i++;


			stringArquivos = stringArquivos->next;
		}
		

		qsort(listaDoc, totalDocumentos, sizeof(documentoRelevancia *), doc_rel_comp);

		printf("\n\n");
		printf("Termo : %s\n", termoOriginal);

		for (int i = 0; i < totalDocumentos; i++) {
			printf("%s : %.6f de relevância\n", listaDoc[i]->value, listaDoc[i]->relevancia);
		}




	} else {
		printf("Opção não reconhecida.\n");
	}

	return 0;
}