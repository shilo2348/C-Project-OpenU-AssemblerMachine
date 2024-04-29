#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"
#include "hash.h"
#include "macro.h"
#include "savedWords.h"
#include "functions.h"
#include "defines.h"
#include "errors.h"


bool preprocessor(const char *file, hashTable macroTable);

#endif