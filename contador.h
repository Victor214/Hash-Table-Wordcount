char * getNextWord(FILE * fp, bool * state);
node** getAllElementsFromHashTable(hashTable* table);
int node_comp(const void* p1, const void* p2);
int doc_rel_comp(const void* p1, const void* p2);
termosList* createTermosList(char* value);
arquivosList* createArquivosList(char* value);
documentoRelevancia* createDocumentoRelevancia(char* value, double relevancia);
int calcFreq (FILE * fp, hashTable * table);