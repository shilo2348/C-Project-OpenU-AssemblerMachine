#include "firstPass.h"

/*
* This function goes line by line in the file and checks the correctness of the code.
* The function inserts the labels into the symbol table with their value.
* If the code is incorrect, an error will be printed on the screen.
*/
bool firstPass(const char *fileName, hashTable macroTable, hashTable tagsTable, int *IC, int *DC){

    char *nameSrc = addingSuffix(fileName, ".am");
    FILE *src = openFile(nameSrc, "r"); 
    char line[MAX_LINE_LENGTH] = {0};
    int lineNumber = 0;
    char word[MAX_LINE_LENGTH];
    int ICCuont = 0;
    bool fileIsValid = true;         /* Flag indicating whether the file is valid */
    *IC = 100;                       /* start adrress of IC */
    *DC = 0;
    

    while (fgets(line, MAX_LINE_LENGTH, src) != NULL){     /* Read each line from the source file */
        int i = 1;
        int cmdType = EOF;
        char cmd[MAX_LINE_LENGTH]; 
        char* result = findIthWord(word, line, i+1, " \t\r\n,");
        lineNumber++;                                            /* Increment line number for error reporting */
        
        if(!isTooLong(line, &fileIsValid, lineNumber))      /* Check if the line is too long */
            continue;
        if(isComment(line)){                                /* Check if the line is a comment, and skip it if true */
            continue;
        }
        if (result != NULL){
            strcpy(cmd, result);
        }

        /* Check if the line contains a label, and process it if true */
        if(isLabel(line))
            checkSymbol(findIthWord(word, line, i++, " \t:"), cmd, true, *IC, *DC, &fileIsValid, lineNumber, tagsTable, macroTable);

        /* Check if the line contains a macro definition, and process it if true */
        if(isDefine(line)){
            if(checkDefine(line, *IC, *DC, &fileIsValid, lineNumber, tagsTable, macroTable)){
                char word3[MAX_LINE_LENGTH];
                char word2[MAX_LINE_LENGTH];
                strcpy(word3, findIthWord(word, line, 3, " \t\r\n="));
                strcpy(word2, findIthWord(word, line, 2, " \t\r\n="));
                    insertHash(tagsTable, createNode(NULL, word2, createData("mdefine", atoi(word3))));
            }
        }
               
        /* Check if the line contains a string definition, and process it if true */
        if(isString(line, i)){
            if(checkString(line, i+1, &fileIsValid, lineNumber, tagsTable, macroTable))
                *DC = strrchr(line, '\"') - strchr(line, '\"'); 
                /* *DC += strlen(findIthWord(word, line, i+1, " \t\r\n"))-1;*/
        }

        /* Check if the line contains data, and process it if true */
        if(isData(line, i)){
            if(checkData(line, i+1, &fileIsValid, lineNumber, tagsTable, macroTable)){
                *DC += sizeOfData(line);
            }
        }

        /* Check if the line contains an instruction, and process it if true */
        if((cmdType =  instructionWord(findIthWord(word, line, i, " \t\r\n,"))) != EOF){
            if(checkInstruction(line, i, cmdType, &ICCuont, &fileIsValid, lineNumber, tagsTable));
                *IC += ICCuont;
        }

        /* Check if the line contains an external declaration, and process it if true */
        if(isExtern(line)){
           checkSymbol(findIthWord(word, line, 2, " \t\r\n"), ".extern", false, *IC, *DC, &fileIsValid, lineNumber, tagsTable, macroTable);
        }

        /* Check if the line contains an illegal word */
        if(isIllegalWord(line, i))
            fileIsValid = error(lineNumber, 7);  
    }
    
    
    addICtoDC(tagsTable, *IC);        /* Add IC to DC where necessary */
    *IC = *IC - 100;
    free(nameSrc);
    fclose(src);
    return fileIsValid;               /* Return whether the file is valid or not */

}