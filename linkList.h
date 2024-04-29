#ifndef LINK_LIST_H
#define LINK_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "file.h"

typedef struct node
{
    struct node* next;
    char *key;
    void *data;     
} node;

typedef struct  data{
    int value;
    char *type;
} data;

node* createNode(node* next, char *key, void *data);

data* createData(char *type, int value);

void freeList(struct node* head, bool structData); /* , void (*freeData)(void*) */

void insertNode(node **head, node *curr);

node* findNode(node *head, char *key);

void printList(node *head);

#endif      /* LINK_LIST_H */


