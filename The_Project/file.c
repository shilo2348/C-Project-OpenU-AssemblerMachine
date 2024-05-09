#include "file.h"

/*  Adding a suffix to the name of a file.     */
char *addingSuffix(const char *fileNameSrc, const char *suffix){
    /* Calculate the length of the new file name */
    int fileNameLen = strlen(fileNameSrc);
    int suffixLen = strlen(suffix);
    int newFileNameLen = fileNameLen + suffixLen + 1; /* +1 for the '\0' terminator */

    /* Allocate memory for the new file name */
    char *fileNameTarget = malloc(newFileNameLen);
    checkAllocation(fileNameTarget);

    /* Copy the original file name to the new file name */
    strcpy(fileNameTarget, fileNameSrc);

    /* Append the suffix to the new file name */
    strcat(fileNameTarget, suffix);
    return fileNameTarget;
}

/* changing a suffix to the name of a file. */
char *changingSuffix(const char *fileNameSrc, const char suffix){
    char *fileNameTarget = malloc(strlen(fileNameSrc) + 2);
        if (fileNameTarget == NULL) {
        printf("Memory allocation failed!\n");
        return NULL;
    }
    strcpy(fileNameTarget, fileNameSrc);
    fileNameTarget[strlen(fileNameSrc)-1] = suffix;
    fileNameTarget[strlen(fileNameSrc)] = '\0';
    return fileNameTarget;
}

FILE* openFile(const char* fileName, const char* mode) {
    FILE* file = fopen(fileName, mode); /* Open the file with the specified mode */
    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", fileName);
        exit(EXIT_FAILURE); /* Exit the program if the file couldn't be opened */
    }
    return file; /* Return the file pointer if opened successfully */
}

void checkAllocation(void* ptr){
    if (ptr == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE); /* Exit the program if memory allocation failed */
    }
}
