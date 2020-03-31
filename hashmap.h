#ifndef HASHMAP_H
#define HASHMAP_H

#define TABLE_INIT_SIZE 10
#define LOAD_FACTOR 0.5

int h_size = TABLE_INIT_SIZE;
int h_items = 0;

typedef struct htable_node {
  char * string;
  int hashcode;
  int freq;
} h_node;


//borrowed from Dan Bernstein
int h_func(char *);


h_node * h_init();

h_node * h_rehash(h_node*);

//inserts string if doesn't exist, adds count if it does
//returns pointer to hash table
h_node * h_add(h_node*, char*, int);

#endif
