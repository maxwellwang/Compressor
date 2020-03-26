all: fileCompressor.c huffman.o
	gcc -o "fileCompressor" -g -Wall -Werror -Wvla fileCompressor.c huffman.o

huffman.o: huffman.c
	gcc -c -Wall -Werror -Wvla huffman.c

clean:
	rm fileCompressor; rm huffman.o
