#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 10  // You can adjust the table size based on your needs

// Node for collision handling (linked list)
typedef struct node {
    char *key;
    int value;
    struct node *next;
} Node;

// Hashmap structure
typedef struct hashmap {
    Node **buckets;
} Hashmap;

unsigned int hashmap_hash(char *key);

Hashmap *hashmap_create();

void hashmap_insert(Hashmap *map, char *key, int value);

int hashmap_search(Hashmap *map, char *key);

void hashmap_delete(Hashmap *map, char *key);

void hashmap_free(Hashmap *map);