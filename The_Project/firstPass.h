#ifndef FIRSTPASS_H
#define FIRSTPASS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "macro.h"
#include "defines.h"
#include "functions.h"
#include "savedWords.h"
#include "errors.h"



bool firstPass(const char *fileName, hashTable macroTable, hashTable tagsTable, int *IC, int *DC);


#endif
