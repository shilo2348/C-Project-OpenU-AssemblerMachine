#include "functions.h"


/* The function find and return the second word from a given line of text. */
char* findIthWord(char *word, const char *line, int i, char *delimiters){
     
     /* Create a copy of the line to avoid modifying the original string */
    char lineCopy[MAX_LINE_LENGTH + 1];  /* Add 1 for the null terminator */
    char *token;
    int j = 1;
    strncpy(lineCopy, line, MAX_LINE_LENGTH);
    lineCopy[MAX_LINE_LENGTH] = '\0';

    token = strtok(lineCopy, delimiters);

    for (; j < i && token != NULL; j++)           /* Use strtok to tokenize the line */
        token = strtok(NULL, delimiters);

    if(token){
        strcpy(word, token);
        return word;
    }
    else
        return NULL;
}
