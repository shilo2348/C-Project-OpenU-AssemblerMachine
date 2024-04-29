#ifndef SECONDPASS_H
#define SECONDPASS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "defines.h"
#include "file.h"
#include "hash.h"
#include "binaryCode.h"
#include "savedWords.h"
#include "functions.h"


bool secondPass(const char *fileName, hashTable macroTable, hashTable tagsTable, int IC, int DC);



#endif