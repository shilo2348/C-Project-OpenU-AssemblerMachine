#ifndef BINARYCODE_H
#define BINARYCODE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "defines.h"
#include "hash.h"
#include "savedWords.h"
#include "functions.h"
#include "file.h"

struct extCall {
    char name[32];
    int adrress;
};

struct symbol {
    char name[32];
    enum {
        sym_data,
        sym_code,
        sym_define,
        sym_extern,
        sym_entry, 
        sym_entry_code,
        sym_entry_data
    }sym_type;
    int define_const;
};

typedef struct binaryCode{
    short code[4096];
    int codeSize;
    short data[4096];
    int dataSize;
    struct extCall ext[100];
    int extSize;
} binaryCode;

int enterCode(char* line, int lineNumber, bool *fileIsValid, hashTable tagsTable, struct binaryCode *binary);

bool isValidLabel(hashTable tagsTable, char *symbol);

int enterData(char* line, int lineNumber, bool *fileIsValid, hashTable tagsTable, struct binaryCode *binary);

int enterString(const char* line, int lineNumber, bool *fileIsValid, hashTable tagsTable, struct binaryCode *binary);

void printbinary(struct binaryCode binary);

void createOb(const char *fileName, int IC, int DC, struct binaryCode binary);

char *encryptedCode(int num);

char encrypted(int num);

void createEnt(const char *fileName, hashTable tagsTable);

void createExt(const char *fileName, struct binaryCode binary);

int decToBinary(int num);






#endif