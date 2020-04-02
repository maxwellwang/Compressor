all: fileCompressor.c demo.o
	gcc -o fileCompressor fileCompressor.c demo.o

demo.o: demo.c
	gcc -c demo.c

clean:
	rm fileCompressor; rm demo.o
