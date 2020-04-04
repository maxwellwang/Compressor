#ifndef HASHMAP_H
#define HASHMAP_H

#define TABLE_INIT_SIZE 10
#define LOAD_FACTOR 0.5

extern int h_size;
extern int h_items;

typedef struct htable_node {
  char * string;
  int hashcode;
  char * freq;
} h_node;


//borrowed from Dan Bernstein
int h_func(char *);

h_node * h_init();

h_node * h_rehash(h_node*, int);

char * h_get(h_node* h_table, char * str);

h_node * h_add_helper(h_node * h_table, char * str, int len, char* count);
//inserts string if doesn't exist, adds count if it does
//returns pointer to hash table
h_node * h_add(h_node*, char*, char*, int);

#endif
