#ifndef HASH_H
#define HASH_H

#include "linkList.h"
#include <string.h>
#include <stdbool.h>

#define HASH_SIZE 200

typedef node *hashTable[HASH_SIZE];

void freeHash(hashTable table, bool structData);

void insertHash(hashTable table, node *curr);

node* findHash(hashTable table, char *key);

int hashFunction(hashTable mp, char* key);

void printHash(hashTable table);






#endif      /* HASH_H */

 
