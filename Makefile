all: fileCompressor.c huffman.o hashmap.o
	gcc -o fileCompressor fileCompressor.c huffman.o hashmap.o

huffman.o: huffman.c
	gcc -c huffman.c

hashmap.o: hashmap.c
	gcc -c hashmap.c

clean:
	rm fileCompressor; rm huffman.o; rm hashmap.o
