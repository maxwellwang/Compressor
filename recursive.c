#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>

//DT_BLK      This is a block device.
//DT_CHR      This is a character device.
//DT_DIR      This is a directory.
//DT_FIFO     This is a named pipe (FIFO).
//DT_LNK      This is a symbolic link.
//DT_REG      This is a regular file.
//DT_SOCK     This is a UNIX domain socket.
//DT_UNKNOWN  The file type is unknown.


//max length of path?
//what tpyes of files?
void recurse(char * dirname) {
  struct dirent *dp;
  DIR * dir_struct = opendir(dirname);
  char * name;
  char * pathname;
  char type;

  if (!dir_struct) {
    printf("Bye %s %s\n", dirname, strerror(errno));
    return;
  }
  
  while ((dp = readdir(dir_struct)) != NULL) { //skipping opening files
    name = dp->d_name;
    type = dp->d_type;
    if (type == DT_DIR) {
      continue;
      if ((strcmp(name,".") * strcmp(name,".."))) {
	//	printf("Directory found: %s\n", name);
	pathname = malloc(strlen(dirname) + strlen(name) + 1 + 1);
	memset(pathname, 0, strlen(dirname) + strlen(name) + 1 + 1);
	memcpy(pathname, dirname, strlen(dirname));
	pathname[strlen(pathname)] = '/';
	memcpy(pathname + strlen(dirname)+1, name, strlen(name));
	recurse(pathname);
	free(pathname);
      }
    } else if (type == DT_REG) {
      printf("File found: %s\n", name);
      FILE * fd = open(name, O_RDONLY);
      char c;
      while (read(fd, &c, 1)) {
	printf("%c", c);
      }
      printf("\n");
    }
  }
}


int main() {
  recurse(".");
  return 0;
}
