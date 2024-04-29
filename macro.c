#include "macro.h"


  
/* Function to check if a line is a macro definition */
int isMacro(char *line) {
    char newLine[MAX_LINE_LENGTH];
    strcpy(newLine, line);
    if(strtok(newLine, " \t\r\n") == NULL)
        return 0;
    return (!strcmp(strtok(newLine, " \t\r\n"), "mcr"));
}


node *savedMacro(hashTable table, char *line){
    char newLine[MAX_LINE_LENGTH];
    strcpy(newLine, line);
    return findHash(table, strtok(newLine, " \t\r\n")); 
}

int emptyLine(char *line){
    char newLine[MAX_LINE_LENGTH];
    strcpy(newLine, line);
    
    if(strtok(newLine, " \t\r\n") == NULL)
        return 1;

    return 0;    
}

Bool checkMacro(char *line, int lineNumber, hashTable macroTable){
    char word[MAX_LINE_LENGTH];
    char macroName[MAX_LINE_LENGTH];

    if(findIthWord(word, line, 2, " \t\r\n") != NULL)
        strcpy(macroName, findIthWord(word, line, 2, " \t\r\n"));

    if(!findIthWord(word, line, 2, " \t\r\n"))            /* Checking if there isn't name for this macro. */
        return error(lineNumber, 1);

    if(findIthWord(word, line, 3, " \t\r\n") != NULL)            /* Checking if there is name after the macro. */
        return error(lineNumber, 1);
    
    else if(!isalpha(macroName[0]))
        return error(lineNumber, 2); 

    else if(savedWord(macroName)) /* Checking if this macro has been saved word. */
        return error(lineNumber, 3); 

    else if(findHash(macroTable, macroName) != NULL) /* Checking if this macro has already been saved before. */
        return error(lineNumber, 4);
    return true;
}