all: fileCompressor.c huffman.o hashmap.o
	gcc -o "fileCompressor" -g -Wall -Werror -Wvla fileCompressor.c huffman.o hashmap.o

huffman.o: huffman.c
	gcc -c -Wall -Werror -Wvla huffman.c

hashmap.o: hashmap.c
	gcc -c -Wall -Werror -Wvla hashmap.c

clean:
	rm fileCompressor; rm huffman.o; rm hashmap.o
