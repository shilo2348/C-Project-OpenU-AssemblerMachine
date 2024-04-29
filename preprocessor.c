#include "preprocessor.h"

/*
 * The function accepts a file, extracts its macros and returns a file with embedded macros.
 * @param file The input assembly file to be preprocessed.
 * @param macroTable The hash table to store macros.
 * @return True if the preprocessing is successful, false otherwise.
 */

bool preprocessor(const char *file, hashTable macroTable){

    char *fileNameSrc = addingSuffix(file, ".as");
    char *fileNameTarget = changingSuffix(fileNameSrc, 'm');
    FILE *src;
    FILE *target;
    bool fileIsValid = true;              /* flag if The macro file is valid. */ 
    char line[MAX_LINE_LENGTH];
    int lineNumber = 0;
    char word[MAX_LINE_LENGTH];
    char *data= (char *)malloc(1);
    
    checkAllocation(data);
    strcpy(data, "\0");
    src = openFile(fileNameSrc, "r"); 
    target = openFile(fileNameTarget, "w");
    free(fileNameSrc);
    free(fileNameTarget);

     /* Read line by line from the input file and process it */
    while (fgets(line, MAX_LINE_LENGTH, src) != NULL) {
        lineNumber++;
        if (!(emptyLine(line)  || isMacro(line)  ||  savedMacro(macroTable, line) != NULL)) {  
            /* If it isn't a macro, write to the output file */
            fputs(line, target);
        }
        else if(emptyLine(line));  
        else if((isMacro(line))){
            char *macroName = findIthWord(word, line, 2, " \t\r\n");        /* Extract macro name */
            if(checkMacro(line, lineNumber, macroTable)){                /* Check if the macro is valid */   
                char word1[MAX_LINE_LENGTH] ={0};
                char word2[MAX_LINE_LENGTH] ={0};
                
                while (fgets(line, MAX_LINE_LENGTH, src)){         /* Read macro body */
                    lineNumber++;
                    strcpy(word1, line);
                    strcpy(word2, line);    
    
                    if(strtok(word2, " \t\r\n") == NULL)
                        continue; 
                    if(strcmp(strtok(word1, " \t\r\n"), "endmcr") != 0){               /* Check if it's the end of the macro */
                        data = (char *)realloc(data, strlen(data) + strlen(line) + 2);  /* Append line to macro data */
                        checkAllocation(data); 
                        strcat(data, line);
                    }                                           
                    else if(strtok(NULL, " \t\r\n") != NULL){     /* Error: missing endmcr */
                            fileIsValid = error(lineNumber, 5); 
                        break;
                    }
                    else
                        break;
                }
                insertHash(macroTable, createNode(NULL , macroName, data));       /* Insert macro into the hash table */
            }
            else
                fileIsValid = false;
        }                
        else{                      
            char *macroName = strtok(line, " \t\r\n");
            char *token = strtok(NULL, " \t\r\n");
            if(token != NULL)
                fileIsValid = error(lineNumber, 6);                 /* Error: invalid macro usage */
            fputs((char *)findHash(macroTable, macroName)->data, target); 
        }                      
    }
    free(data);
    fclose(src);
    fclose(target);

    return fileIsValid;                     
}