#include <stdio.h>
#include <stdlib.h>
#include "hashmap.h"
#include <string.h>

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
    (p[i]).freq = 0;
  }
  return p;  
}

h_node * h_rehash(h_node * h_table);


//inserts string if doesn't exist, adds count if it does
//returns pointer to hash table
h_node * h_add(h_node * h_table, char * str, int count) {
  float lf = LOAD_FACTOR;
  if ((float) h_items / (float) h_size >= lf) {
    h_table = h_rehash(h_table);
  }  
  int hash_code = h_func(str);
  int i = 0, index = hash_code % h_size;
  while (h_table[index].string != NULL && strcmp(h_table[index].string, str) != 0) { 
    index = (hash_code + i++) % h_size;
  }
  (h_table[index]).string = str; //should I instead malloc something new? or nah
  (h_table[index]).hashcode = hash_code;
  (h_table[index]).freq += count;
  h_items += h_table[index].freq == count ? 1 : 0;
  return h_table;
}

h_node * h_add_helper(h_node * h_table, char * str, int len, int count) {
  char * string = malloc(len+1);
  memset(string, 0, len+1);
  memcpy(string, str, len);
  h_add(h_table, char * string, count);
}

h_node * h_rehash(h_node * h_table) {
  h_size *= 2;
  h_items = 0;
  h_node * h_tmp = h_init();
  int i = 0;
  for (; i < h_size; i++) {
    (h_tmp[i]).string = NULL;
    (h_tmp[i]).hashcode = 0;
    (h_tmp[i]).freq = 0;
  }
  for (i = 0; i < h_size/2; i++) {
    if ((h_table)[i].string != NULL) {
      h_tmp = h_add(h_tmp, (h_table)[i].string, (h_table)[i].freq);
    }
  }
  free(h_table);
  return h_tmp;
}
