#include "hash.h"

/* Function to free memory occupied by the hash table */
void freeHash(hashTable table, bool structData){
    int i = 0;
    for(; i < HASH_SIZE; i++){
        freeList(table[i], structData);        
    }
}

/* Function to insert a node into the hash table */
void insertHash(hashTable table, node *curr){
    int bucketIndex = hashFunction(table, curr->key);
    insertNode(&table[bucketIndex], curr);        
}

/* Function to find a node by key in the hash table */
node* findHash(hashTable table, char *key){
    int bucketIndex = hashFunction(table, key);
    if(table[bucketIndex] != NULL){
        return findNode(table[bucketIndex], key);
    }
    return NULL;
}

/* Hash function to calculate the bucket index */
int hashFunction(hashTable table, char* key){
    int bucketIndex = 0, i = 0;
    if(key == NULL)
        return 0;
    
    for(;i < strlen(key); i++){
    bucketIndex += key[i];  
    }
    bucketIndex = bucketIndex % HASH_SIZE;

    if (bucketIndex < 0)
        bucketIndex = -bucketIndex;
    
    return bucketIndex;
}

/* Function to print the hash table */
void printHash(hashTable table){
    int i = 0;
    for (; i < HASH_SIZE; i++){
        printList(table[i]);
    }
    
}