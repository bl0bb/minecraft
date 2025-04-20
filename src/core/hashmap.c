#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashmap.h"

// Hash function to compute the index for a key
unsigned int hashmap_hash(char *key) {
    unsigned int hash_value = 0;
    while (*key) {
        hash_value = (hash_value << 5) + *key++;  // Simple hash function
    }
    return hash_value % TABLE_SIZE;
}

// Function to create a new hashmap
Hashmap *hashmap_create() {
    Hashmap *map = malloc(sizeof(Hashmap));
    map->buckets = malloc(sizeof(Node*) * TABLE_SIZE);
    for (int i = 0; i < TABLE_SIZE; i++) {
        map->buckets[i] = NULL;  // Initialize each bucket to NULL
    }
    return map;
}

// Function to insert a key-value pair into the hashmap
void hashmap_insert(Hashmap *map, char *key, int value) {
    unsigned int index = hashmap_hash(key);
    Node *new_node = malloc(sizeof(Node));
    new_node->key = strdup(key);  // Duplicate the key
    new_node->value = value;
    new_node->next = map->buckets[index];  // Insert at the beginning of the list
    map->buckets[index] = new_node;
}

// Function to search for a value by its key
int hashmap_search(Hashmap *map, char *key) {
    unsigned int index = hashmap_hash(key);
    Node *current = map->buckets[index];
    while (current) {
        if (strcmp(current->key, key) == 0) {
            return current->value;  // Return the value if found
        }
        current = current->next;
    }
    return -1;  // Return -1 if key is not found
}

// Function to delete a key-value pair from the hashmap
void hashmap_delete(Hashmap *map, char *key) {
    unsigned int index = hashmap_hash(key);
    Node *current = map->buckets[index];
    Node *previous = NULL;

    while (current) {
        if (strcmp(current->key, key) == 0) {
            if (previous) {
                previous->next = current->next;  // Skip the node to delete it
            } else {
                map->buckets[index] = current->next;  // Remove the first node
            }
            free(current->key);
            free(current);
            return;
        }
        previous = current;
        current = current->next;
    }
}

// Function to free the hashmap
void hashmap_free(Hashmap *map) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node *current = map->buckets[i];
        while (current) {
            Node *temp = current;
            current = current->next;
            free(temp->key);
            free(temp);
        }
    }
    free(map->buckets);
    free(map);
}