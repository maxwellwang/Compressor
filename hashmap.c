#include <stdio.h>
#include <stdlib.h>
#include "hashmap.h"
#include <string.h>
//

int h_size = TABLE_INIT_SIZE;
int h_items = 0;


//borrowed from Dan Bernstein
int h_func(char * str) {
  unsigned long hash = 5381;
  int c = 0;
  while (c = *str++) {
    hash = ((hash << 5) + hash) + c;
  }
  return hash % h_size;
}


h_node * h_init() {
  h_node * p = malloc(h_size * sizeof(h_node));
  int i = 0;
  for (; i < h_size; i++) {
    (p[i]).string = NULL;
    (p[i]).hashcode = 0;
    (p[i]).freq = malloc(20);;
  }
  return p;  
}

//h_node * h_rehash(h_node * h_table);

//inserts string if doesn't exist, adds count if it does
//returns pointer to hash table
h_node * h_add(h_node * h_table, char * str, char * count, int num) {
  float lf = LOAD_FACTOR;
  if ((float) h_items / (float) h_size >= lf) {
    h_table = h_rehash(h_table, num);
  }  
  int hash_code = h_func(str);

  int i = 0, index = hash_code % h_size;
  while (h_table[index].string != NULL && strcmp(h_table[index].string, str) != 0) { 
    index = (hash_code + i++) % h_size;
  }
  (h_table[index]).string = str; //should I instead malloc something new? or nah
  (h_table[index]).hashcode = hash_code;
  if (!num) {
    (h_table[index]).freq = count;
  } else {
    sprintf((h_table[index]).freq, "%d", atoi((h_table[index]).freq) + atoi(count));
  }
  h_items += atoi(h_table[index].freq) == atoi(count) ? 1 : 0;
  return h_table;
}

char * h_get(h_node * h_table, char * str) {
  int hash_code = h_func(str);
  int i = 0, index = hash_code % h_size;
  while (h_table[index].string != NULL && strcmp(h_table[index].string, str) != 0) { 
    index = (hash_code + i++) % h_size;
  }
  //  printf("H%s\n", h_table[index].freq);
  return (h_table[index]).freq;
}

h_node * h_add_helper(h_node * h_table, char * str, int len, char * count) {
  char * string = malloc(len+1);
  memset(string, 0, len+1);
  memcpy(string, str, len);
  return h_add(h_table, string, count, 1);
}

h_node * h_rehash(h_node * h_table, int num) {
  h_size *= 2;
  h_items = 0;
  h_node * h_tmp = h_init();
  int i = 0;
  for (; i < h_size; i++) {
    (h_tmp[i]).string = NULL;
    (h_tmp[i]).hashcode = 0;
    (h_tmp[i]).freq = malloc(16);
  }
  for (i = 0; i < h_size/2; i++) {
    if ((h_table)[i].string != NULL) {
      h_tmp = h_add(h_tmp, (h_table)[i].string, (h_table)[i].freq, num);
    }
  }
  free(h_table);
  return h_tmp;
}
