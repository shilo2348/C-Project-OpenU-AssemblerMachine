#ifndef MACRO_H
#define MACRO_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "defines.h"
#include "functions.h"
#include "errors.h"
#include "savedWords.h"


#define MAX_TAG_LENGTH 31


struct macro{
    char macroName[MAX_TAG_LENGTH];
    int macroStart;
    int macroEnd;
};

int isMacro(char *line);

node *savedMacro(hashTable table, char *line);

int emptyLine(char *line);

Bool checkMacro(char *line, int lineNumber, hashTable macroTable);

#endif