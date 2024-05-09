#include "handleMain.h"


/*
* The handelMain handles the file in 3 steps:
* Layout of macros.
* First pass on the code.
* Second pass over the code.
*/

int handleMain(const char *fileName){
    char name[MAX_LINE_LENGTH];
    hashTable macroTable = {0};
    hashTable tagsTable = {0};
    int IC = 0;
    int DC = 0;
    strcpy(name, fileName);
    /* Run preprocessor */    
    if(preprocessor(fileName, macroTable)){    
        if(firstPass(fileName, macroTable, tagsTable, &IC, &DC)){ /* If preprocessor successful, run first pass */
            secondPass(name, macroTable, tagsTable, IC, DC);      /* If first pass successful, run second pass */
        }
    }
    else{                /* Clean up temporary files if preprocessor failed */
        char *nameSrc = addingSuffix(fileName, ".am");
        if (remove(nameSrc) == 0){
            printf("%s removed successfully.\n", nameSrc);
        } 
        else {
            perror("Error removing file");
        }
        free(nameSrc);      
    }
    /* Free memory allocated for hash tables */
    freeHash(macroTable, false);
    freeHash(tagsTable, true);
   
    return 0;
}

