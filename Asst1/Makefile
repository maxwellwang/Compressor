all: fileCompressor.c hashmap.o demo.o
	gcc -o fileCompressor fileCompressor.c hashmap.o demo.o

demo.o: demo.c
	gcc -c demo.c

hashmap.o: hashmap.c
	gcc -c hashmap.c

clean:
	rm fileCompressor; rm demo.o; rm hashmap.o
