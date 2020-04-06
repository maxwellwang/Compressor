#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
	char* buffer = malloc(100);
	char* temp = NULL;
	int i;
	char* head = buffer;
	int size = 10;
	int tokenLength = 0;
	for (i = 0; i < 100; i++) {
		if (tokenLength + 1 > size) {
			temp = malloc(size * 2);
			memcpy(temp, buffer, size);
			free(buffer);
			buffer = temp;
			size *= 2;
			temp = NULL;
		}
		head = buffer + tokenLength;
		*head = '0';
		tokenLength++;
		printf("%s\n", buffer);
	}
	return 0;
}
