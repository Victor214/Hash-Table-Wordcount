all: main.c tabelahash.c
	gcc main.c tabelahash.c contador.c -lm -o indexer
clean: 
	$(RM) indexer