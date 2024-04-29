#include "secondPass.h"
/*
* The function goes through the code and encodes it in encrypted base 4.
* The function produces 3 files:
* A file with the extension ob- contains the encrypted base 4 machine language code.
* A file with the extension ent- contains the symbols that are defined as entries.
* A file with the extension ext- contains the symbols that are defined as extern.
*/
bool secondPass(const char *fileName, hashTable macroTable, hashTable tagsTable, int IC, int DC){
    char *nameSrc = addingSuffix(fileName, ".am");
    FILE *src = openFile(nameSrc, "r");          /* Open the source file ".am" for reading */
    char line[MAX_LINE_LENGTH] = {0};
    char word[MAX_LINE_LENGTH];
    char word1[MAX_LINE_LENGTH];
    char word2[MAX_LINE_LENGTH];
    int entryCuont = 0;
    int lineNumber = 0;
    bool fileIsValid = true;
    int i = 0;
    struct binaryCode binary;         /* Struct to hold binary code */

    /* Initialize binary code struct */
    binary.codeSize = 0;
    binary.dataSize = 0;
    binary.extSize = 0;

    /* Initialize code and data arrays in binary code struct */
    for(;i < 4096; i++){
        binary.code[i] = 0;
        binary.data[i] = 0;
    }
    
    while (fgets(line, MAX_LINE_LENGTH, src) != NULL){           /* Read each line from the source file */
        lineNumber++;
        strcpy(word1, findIthWord(word, line, 1, " \t\r\n,:#"));              /* Extract the first and second words from the line */  
        if(findIthWord(word, line, 2, " \t\r\n,:#") != NULL)
            strcpy(word2, findIthWord(word, line, 2, " \t\r\n,:#"));     
        
        if(isComment(line)){             /* Check if the line is a comment, and skip it if true */
            continue;
        }

        /* Check if the line contains an entry declaration */
        else if(isEntry(line))
            entryCuont += checkEntry(line, &fileIsValid, lineNumber, tagsTable); 

        /* Check if the line contains an instruction, and process it if true */
        else if(instructionWord(word1) != -1 || (word2 != NULL && instructionWord(word2) != -1))
            binary.codeSize += enterCode(line, lineNumber, &fileIsValid, tagsTable, &binary);    
    
        /* Check if the line contains data, and process it if true */
        else if(isData(line, 1)|| isData(line, 2))
            binary.dataSize += enterData(line, lineNumber, &fileIsValid, tagsTable, &binary);

        /* Check if the line contains a string, and process it if true */
        else if(isString(line, 1)|| isString(line, 2))
            binary.dataSize += enterString(line, lineNumber, &fileIsValid, tagsTable, &binary);

    }
    /* Create the output files */
    createOb(fileName, IC, DC, binary);
    if(entryCuont > 0)
        createEnt(fileName, tagsTable);
    
    if(binary.extSize > 0)
        createExt(fileName, binary);

       
    free(nameSrc);
    fclose(src);
    return fileIsValid;
} 