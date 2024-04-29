#include "linkList.h"

/* Function to create a new node */
node* createNode(node *next, char *key, void *data){
    struct node* curr = (struct node*)malloc(sizeof(struct node));
    char *newKey = (char *)malloc(strlen(key)+1);
    checkAllocation(curr);
    checkAllocation(newKey);
                               
    strcpy(newKey, key);    /* Copy the key and assign other values */
    curr->key = newKey;
    curr->data = data;
    curr->next = next;

    return curr;
}

/* Function to create a new data structure */
data* createData(char *type, int value){
    struct data* curr = (struct data*)malloc(sizeof(struct data));
    char *newType = (char *)malloc(strlen(type) + 10);
    checkAllocation(curr);
    checkAllocation(newType);
    
    strcpy(newType, type);   /* Copy the type and assign other values */
    curr->type = newType;
    curr->value = value;
    return curr;
}

/* Function to free memory occupied by the linked list */
void freeList(struct node *head, bool structData){
    while (head != NULL) {
        node* temp = head;
        head = head->next;
        free(temp->key);        /* Free key memory */
        
        if(structData){                    /* Free data structure memory if structData is true */
            free(((struct data*)(temp->data))->type);
            free(temp->data);
        }      
        free(temp);
    }
}

/* Function to insert a node into the linked list */
void insertNode(struct node **head, struct node *curr) {
    if (*head == NULL){
        *head = curr;                      /* If the list is empty, make the new node the head */
    }
    else{                       /* If the Linked List isn't empty, then make the new node as tail */
        struct node *last = *head;

        while (last->next != NULL) {
            last = last->next;
        }
        last->next = curr;
    } 
}

/* Function to find a node by key */
node* findNode(node *head, char *key){
    struct node *curr = head;
    while (curr != NULL){
        if(strcmp(curr->key, key) == 0){
            return curr;
        }    
        curr = curr->next;
    }
    return NULL;                                    
}

/* Function to print the linked list */
void printList(node *head){
    while(head != NULL){
        printf("%s\t%s\t\t%d\n", head->key, ((struct data*) (head->data))->type, ((struct data*) (head->data))->value);
        head = head->next;
    }
}