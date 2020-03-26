#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

// Check if flags make sense
int validFlagCombo(int buildCodebook, int compress, int decompress, int recursive) {
	int sum = buildCodebook + compress + decompress + recursive;
	if (sum == 1) {
		return buildCodebook || compress || decompress;
	} else if (sum == 2) {
		return recursive;
	}
	return 0;
}

int main(int argc, char** argv) {
	// Interpret command
	if (argc < 3 || argc > 5) {
		printf("Expected 3-5 arguments, received %d\n", argc);
		exit(EXIT_FAILURE);
	}
	int buildCodebook = 0, compress = 0, decompress = 0, recursive = 0;
	int argCounter;
	for (argCounter = 1; argCounter < argc; argCounter++) {
		if (argv[argCounter][0] == '-' && argCounter != argc - 1) {
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
					printf("Expected -b, -c, -d, or -R, received -%c\n", argv[argCounter][1]);
					exit(EXIT_FAILURE);
			}
		} else {
			if (!validFlagCombo(buildCodebook, compress, decompress, recursive)) {
				// flags do not make sense
				printf("Expected either -b/-c/-d or -R -b/-R -c/-R -d, received invalid flag combination\n");
				exit(EXIT_FAILURE);
			}
		}
	}
	
	return 0;
}
