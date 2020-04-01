#include <stdio.h>
#include <stdlib.h>
#include "demo.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

// Reads file to populate hashmap w/ tokens and frequencies
void populateHashmap(int file, h_node* table) {
	char c = '?';
	char* buffer = (char*)malloc(10);
	if (!buffer) {
		printf("Error: Malloc failed\n");
		exit(EXIT_FAILURE);
	}
	memset(buffer, 0, 10);
	char* nextBuffer; // double the buffer if it isn't long enough
	int tokenLength = 0;
	int readingWhitespace = 0; // reading whitespace until non-whitespace, or reading non-whitespace until whitespace
	int size = 10; // buffer size
	char* head = buffer; // where to write next char
	int status = read(file, &c, 1);
	while (status) {
		if (status == -1 && errno == EINTR) {
			// interrupted, try again
			while (status == -1 && errno == EINTR) {
				status = read(file, &c, 1);
			}
		}
		if (ISWHITESPACE(c)) {
			
		}
	}
}

int main(int argc, char** argv) {
	if (argc < 3) {
		// Less than 3 arguments -> error
		printf("Error: Expected at least three arguments, received only %d\n", argc);
		exit(EXIT_FAILURE);
	}
	int buildCodebook = 0, compress = 0, decompress = 0, recursive = 0, flagSection = 1, numFlags = 0;
	int file = 0, codebook = 0;
	DIR* dir = NULL;
	int argCounter = 1;
	// Loop through arguments to determine course of action
	for (argCounter = 1; argCounter < argc; argCounter++) {
		if (argv[argCounter][0] == '-') {
			// Flag detected
			numFlags++;
			if (!flagSection) {
				// Flag detected after file/directory -> error
				printf("Error: Expected flags to come before files/directories, received flag after file/directory\n");
				exit(EXIT_FAILURE);
			}
			if (strlen(argv[argCounter]) > 2) {
				printf("Error: Expected -b, -c, -d, or -R, received %s\n", argv[argCounter]);
				exit(EXIT_FAILURE);
			}
			switch (argv[argCounter][1]) {
				case 'b':
					buildCodebook = 1;
					break;
				case 'c':
					compress = 1;
					break;
				case 'd':
					decompress = 1;
					break;
				case 'R':
					recursive = 1;
					break;
				default:
					// Invalid flag detected -> error
					printf("Error: Expected -b, -c, -d, or -R, received %s\n", argv[argCounter]);
					exit(EXIT_FAILURE);
			}
		} else {
			// File/directory detected
			flagSection = 0;
			if (!buildCodebook && !compress && !decompress) {
				// One of these flags is required but none were found -> error
				if (argCounter == 1) {
					// File/directory is where flag should be -> error
					printf("Error: Expected -b, -c, -d, or -R to be second argument, received file/directory %s\n", argv[argCounter]);
					exit(EXIT_FAILURE);
				}
				if (argCounter == 2 && recursive) {
					// File/directory is where flag should be (recursive flag prior) -> error
					printf("Error: Expected -b, -c, or -d to be third argument (due to recursive flag as second argument), received "
						"file/directory %s\n", argv[argCounter]);
					exit(EXIT_FAILURE);
				}
				if (recursive) {
					printf("Error: Expected -b, -c, or -d to be one of the flags, received -R\n");
				} else {
					printf("Error: Expected -b, -c, or -d to be one of the flags, received no flags\n");
				}
				exit(EXIT_FAILURE);
			}
			if (compress || decompress) {
				// Expecting file/directory and then codebook file
				if (argc - numFlags != 3) {
					// Should have one file/directory and one file but different amount is detected -> error
					printf("Error: Expected one file/directory and then one codebook file, received %d files/directories\n", argc
						- numFlags - 1);
					exit(EXIT_FAILURE);
				}
				if (argCounter == argc - 2) {
					// Expecting file/directory to compress/decompress
					if (recursive) {
						// Expecting directory
						dir = opendir(argv[argCounter]);
						if (!dir) {
							// Open failed -> error
							printf("Error: Expected to open %s directory, failed to open\n", argv[argCounter]);
							exit(EXIT_FAILURE);
						}
					} else {
						// Expecting file
						file = open(argv[argCounter], O_RDONLY | O_NONBLOCK);
						if (file == -1) {
							// Open failed -> error
							printf("Error: Expected to open %s file, failed to open\n", argv[argCounter]);
							exit(EXIT_FAILURE);
						}
					}
				} else {
					// Expecting codebook file
					codebook = open(argv[argCounter], O_RDONLY);
					if (codebook == -1) {
						// Open failed -> error
						printf("Error: Expected to open %s file, failed to open\n", argv[argCounter]);
						exit(EXIT_FAILURE);
					}
				}
			} else {
				// Expecting one file/directory
				if (argc - numFlags != 2) {
					// Should have one file/directory but different amount is detected -> error
					printf("Error: Expected one file/directory, received %d files/directories\n", argc - numFlags - 1);
					exit(EXIT_FAILURE);
				}
				if (recursive) {
					// Expecting directory
					dir = opendir(argv[argCounter]);
					if (!dir) {
						// Open failed -> error
						printf("Error: Expected to open %s directory, failed to open\n", argv[argCounter]);
						exit(EXIT_FAILURE);
					}
				} else {
					// Expecting file
					file = open(argv[argCounter], O_RDONLY);
					if (file == -1) {
						// Open failed -> error
						printf("Error: Expected to open %s file, failed to open\n", argv[argCounter]);
						exit(EXIT_FAILURE);
					}
				}
			}
		}
	}
	if (argc - numFlags == 1) {
		// No files/directories detected -> error
		printf("Error: Expected at least one file/directory, received none\n");
		exit(EXIT_FAILURE);
	}
	
	// Execute desired command
	Heap* aHeap = NULL;
	if (recursive) {
		// Descend through directory and recursively execute command
		if (buildCodebook) {
		
		} else if (compress) {
		
		} else if (decompress) {
		
		}
	} else {
		// Execute command on file (possibly using codebook)
		if (buildCodebook) {
		
		} else if (compress) {
		
		} else if (decompress) {
		
		}
	}
	
	// Free all nodes
	freeHeap(aHeap);
	// Close all opened files/directories
	if (file) {
		close(file);
	}
	if (codebook) {
		close(codebook);
	}
	if (dir) {
		closedir(dir);
	}
	return 0;
}
