#ifndef SAVEDWORDS_H
#define SAVEDWORDS_H

#include <string.h>
#include <ctype.h>
#include "defines.h"
#include "functions.h"
#include "hash.h"
#include "errors.h"

bool isComment(char *line);

bool isTooLong(char *line, bool *fileIsValid, int lineNumber);

bool isDefine(char *line);

bool checkDefine(char *line, int IC, int DC, bool *fileIsValid, int lineNumber, hashTable tagsTable, hashTable macroTable);

bool checkSymbol(char *symbol, char *cmd, bool label, int IC, int DC, bool *fileIsValid, int lineNumber, hashTable tagsTable, hashTable macroTable);

bool checkCmd(char *symbol1, char *cmd, bool label, int IC, int DC, bool *fileIsValid, int lineNumber, hashTable tagsTable);

bool checkEqual(char *line, bool *fileIsValid, int lineNumber);

bool isNumber(char *str);

bool isLabel(char *line);

bool isString(char *line, int i);

bool checkString(char *line, int i, bool *fileIsValid, int lineNumber, hashTable tagsTable, hashTable macroTable);

bool isData(char *line, int i);

bool checkData(char *line, int i, bool *fileIsValid, int lineNumber, hashTable tagsTable, hashTable macroTable);

bool isConst(hashTable tagsTable, char *symbol);

int sizeOfData(char line[]);

int savedWord(const char *word);

bool isExtern(char *line);

bool isEntry(char *line);

int checkEntry(char *line, bool *fileIsValid, int lineNumber, hashTable tagsTable);

int instructionWord(const char *word);

bool checkInstruction(char *line, int i, int cmdType, int *ICCuont, bool *fileIsValid, int lineNumber, hashTable table);

bool checkParams(int cmdType, char *param1, char *param2, hashTable table);

int sizeOfInstruction(int typeOfParam1, int typeOfParam2);

bool checkComma(char *line, int i, int cmdType, char *param2);

bool chekNumParams(int cmdType, char *param1, char *param2);

int typeOfParam(char param[], hashTable table);

int numOfOperands(int i);

bool checkBraket(char *param);

int promptWord(const char *word);

int isRegister(const char *word);

void addICtoDC(hashTable tagsTable, int IC);

bool isIllegalWord(char* line, int i);

#endif
