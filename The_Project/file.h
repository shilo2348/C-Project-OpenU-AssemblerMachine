#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*  Adding a suffix to the name of a file.     */
char *addingSuffix(const char *fileNameSrc, const char *suffix);

/* changing a suffix to the name of a file. */
char *changingSuffix(const char *fileNameSrc, const char suffix);

FILE* openFile(const char* fileName, const char* mode);

void checkAllocation(void* ptr);

#endif
