#include <stdio.h>
#include <stdlib.h>
#include "hashmap.h"
#include "demo.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

int escapeLength = 1; // '!' is default, add more '!' to avoid confusion with '!'s appearing in files

char * readFile(char * filename) {
  int buff_size = 4096;
  char * huff_buffer = (char*)malloc(buff_size);
  if (!huff_buffer) {
  	printf("Error: Malloc failed\n");
  	exit(EXIT_FAILURE);
  }
  memset(huff_buffer, 0, 4096);
     
  int huff_read = 0;
  int huff_fd = open(filename, O_RDONLY);
  int huff_status;

  do {
    huff_status = read(huff_fd, huff_buffer+huff_read, buff_size-1-huff_read);
    huff_read += huff_status;
    if (huff_status == 0) {
      break;
    }
    char * huff_tmp = (char*)malloc(buff_size*2);
    if (!huff_tmp) {
    	printf("Error: Malloc failed\n");
    	exit(EXIT_FAILURE);
    }
    memset(huff_tmp, 0, buff_size*2);
    memcpy(huff_tmp, huff_buffer, huff_read);
    free(huff_buffer);
    huff_buffer = huff_tmp;
    buff_size *= 2;
  }
  while (huff_status > 0);
  close(huff_fd);
  return huff_buffer;
}

// Reads file to populate hashmap w/ tokens and frequencies, also sets escapeLength
h_node * populateHashmap(char * filename, h_node* table) {
  int file = open(filename, O_RDONLY);
  if (file == -1) {
    // Open failed -> error
    printf("Error: Expected to open %s file, failed to open\n", filename);
    exit(EXIT_FAILURE);
  }
  
  char c = '?';
  char pastC = '?'; // need this to count consecutive escape chars
  char* buffer = (char*)malloc(10);
  if (!buffer) {
    printf("Error: Malloc failed\n");
    exit(EXIT_FAILURE);
  }
  memset(buffer, '\0', 10);
  char* nextBuffer = NULL; // double the buffer if it isn't long enough
  int tokenLength = 0;
  int size = 10; // buffer size
  char* head = buffer; // where to write next char
  int status = read(file, &c, 1);
  int readingWhitespace = ISWHITESPACE(c); // read whitespace until non-whitespace, read non-whitespace until whitespace
  int consecEscapes = 1;
  int maxConsecEscapes = consecEscapes; // escape length (# of !s) must be greater than this to avoid confusion
  int foundEscapeChar = 0;
  while (status) {
    if (status == -1 && errno == EINTR) {
      // interrupted, try again
      status = read(file, &c, 1);
      continue;
    }
    if (c == '!') {
    	foundEscapeChar = 1;
    }
    if (c == '!' && pastC == '!') {
    	consecEscapes++;
    } else if (c != '!' && pastC == '!') {
    	if (consecEscapes > maxConsecEscapes) {
    		maxConsecEscapes = consecEscapes;
    	}
    	consecEscapes = 1;
    }
    if (readingWhitespace == !ISWHITESPACE(c)) { // change from whitespace to non-whitespace or vice versa
      // load current token into hashmap
      table = h_add_helper(table, buffer, tokenLength, 1);
      tokenLength = 0;
      head = buffer; // ready to read next token
    }
    readingWhitespace = ISWHITESPACE(c);
    // add char to buffer, resize if necessary
    if (tokenLength + 1 > size) {
      // reallocate memory
      size *= 2;
      nextBuffer = (char*)malloc(size);
      memcpy(nextBuffer, buffer, size);
      free(buffer);
      buffer = nextBuffer;
      nextBuffer = NULL;
      head = buffer + tokenLength;
    }
    *head = c;
    head++;
    tokenLength++;
    pastC = c;
    status = read(file, &c, 1);
  }
  if (consecEscapes > maxConsecEscapes) {
  	maxConsecEscapes = consecEscapes;
  }
  escapeLength = maxConsecEscapes + 1;
  if (!foundEscapeChar) {
  	escapeLength = 1;
  }
  // load in last token
  table = h_add_helper(table, buffer, tokenLength, 1);
  free(buffer);
  return table;
}

// Recursively navigates Huffman tree to populate codebook
void recursivePopulate(int codebook, Node* aNode, char* pathcode, char* head, int size, int pathcodeLength) {
	char* temp;
	if (!aNode) {
		return;
	}
	// go left
	*head = '0';
	if (pathcodeLength + 1 > size) {
		// double pathcode length
		temp = (char*)malloc(size * 2);
		if (!temp) {
			printf("Error: Malloc failed\n");
			exit(EXIT_SUCCESS);
		}
		memcpy(temp, pathcode, size);
		size *= 2;
		free(pathcode);
		pathcode = temp;
		head = pathcode + pathcodeLength;
		temp = NULL;
	}
	pathcodeLength++;
	head++;
	recursivePopulate(codebook, aNode->left, pathcode, head, size, pathcodeLength);
	// back up
	pathcodeLength--;
	head--;
	// Use token and pathcode, the actually important part
	write(codebook, pathcode, pathcodeLength);
	write(codebook, "\t", 1);
	write(codebook, aNode->token, aNode->tokenLength);
	write(codebook, "\n", 1);
	// go right
	*head = '1';
	if (pathcodeLength + 1 > size) {
		// double pathcode length
		temp = (char*)malloc(size * 2);
		if (!temp) {
			printf("Error: Malloc failed\n");
			exit(EXIT_SUCCESS);
		}
		memcpy(temp, pathcode, size);
		size *= 2;
		free(pathcode);
		pathcode = temp;
		head = pathcode + pathcodeLength;
		temp = NULL;
	}
	pathcodeLength++;
	head++;
	recursivePopulate(codebook, aNode->right, pathcode, head, size, pathcodeLength);
	// back up
	pathcodeLength--;
	head--;
	return;
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
  char * dirname;
  char * filename;
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
	  dirname = argv[argCounter];
	  if (!dir) {
	    // Open failed -> error
	    printf("Error: Expected to open %s directory, failed to open\n", argv[argCounter]);
	    exit(EXIT_FAILURE);
	  }
	} else {
	  // Expecting file
	  filename = argv[argCounter];
//	  file = open(argv[argCounter], O_RDONLY);
//	  if (file == -1) {
//	    // Open failed -> error
//	    printf("Error: Expected to open %s file, failed to open\n", argv[argCounter]);
//	    exit(EXIT_FAILURE);
//	  }
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
  Heap* aHeap = (Heap*)malloc(sizeof(aHeap));
  aHeap->finalIndex = -1; // it increments when insertNode is called
  aHeap->size = 100;
  aHeap->heap = (Node**)malloc(sizeof(Node*) * aHeap->size);
  h_node* table = h_init();
  if (recursive) {
    // Descend through directory and recursively execute command
    if (buildCodebook) {
      printf("Looking in: %s\n", dirname);
      recurse(dirname);
      exit(0);			
    } else if (compress) {
		
    } else if (decompress) {
		
    }
  } else {
    // Execute command on file (possibly using codebook)
    if (buildCodebook) {
		table = populateHashmap(filename, table);
      	Node* temp;
      int i;
      	for (i = 0; i < h_size; i++) {
			if (table[i].string) {
			  // create node and insert into heap
			  //printf("[%s]\n", table[i].string);
			  temp = makeTokenNode(table[i].string, strlen(table[i].string), table[i].freq);
			  insertNode(aHeap, temp);
			}
   	   }
      codebook = open("./HuffmanCodebook", O_RDWR | O_CREAT, 00600);
      // escape sequence
      int ind;
      for (ind = 0; ind < escapeLength; ind++) {
      	write(codebook, "!", 1);
      }
      write(codebook, "\n", 1);
      char* pathcode = (char*)malloc(10);
	char* head = pathcode;
	int size = 10;
	int pathcodeLength = 0;
      recursivePopulate(codebook, (aHeap->heap)[0], pathcode, head, size, pathcodeLength);
    } else if (compress) {
      //make it work for any filename
      char * file = readFile("HuffmanCodebook");
      int i = 0;
      for (; i < strlen(file); i++) {
	printf("[%d]", file[i]);
      }
    } else if (decompress) {
		
    }
  }
  // Free all nodes
  if (aHeap) {
    freeHeap(aHeap);
  }
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
