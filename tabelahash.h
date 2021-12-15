hashTable* createHashTable (int tamanho);
int hashKey(char * key, int tamanho);
node * createNode( char * key );
node * searchHashTable( hashTable* table, char * key );
void incrementHashTable( hashTable* table, char * key );