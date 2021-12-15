#ifndef __structs__
#define __structs__

typedef struct nodo {
	char *key;
	int value;
	struct nodo *next;
} node;

typedef struct {
	int tamanho;
	int tamanho_atual;
	node **list;
} hashTable;

typedef struct listaTermo {
	char * value;
	int nDocs; // Número de documentos em que este termo está presente.
	struct listaTermo *next;
} termosList;

typedef struct listaArquivo {
	char * value;
	hashTable * table; // Tabela hash correspondente a contagem de todas as palavras do arquivo.
	int countPalavras; // Número de palavras do arquivo todo.
	struct listaArquivo *next;
} arquivosList;

typedef struct {
	char * value;
	double relevancia;
} documentoRelevancia;

#endif