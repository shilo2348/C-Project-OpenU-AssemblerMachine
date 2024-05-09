#include "binaryCode.h"

/* Function to process an instruction line and enter its binary representation into the binary code */
int enterCode(char* line, int lineNumber, bool *fileIsValid, hashTable tagsTable, struct binaryCode *binary){
    char word[MAX_LINE_LENGTH];
    char currWord[MAX_LINE_LENGTH] = {0};
    int i = 1;
    int j = 1;
    int lineCuont = 1;
    int registerNum = 0;
    int numOfOperand;

    if(isLabel(line))        /* Determine the number of operands for the instruction */
        i++;

    numOfOperand = numOfOperands(instructionWord(findIthWord(word, line, i, " \t\r\n")));
    
    /* Enter the opcode into the binary code */
    (*binary).code[(*binary).codeSize] = instructionWord(findIthWord(word, line, i, " \t\r\n"));
    (*binary).code[(*binary).codeSize] <<= 2;
    
    /* If the instruction has no operands, return */
    if(numOfOperand == 0){
            (*binary).code[(*binary).codeSize] <<= 4;
            return 1;
    }    
    i++;
    
    /* Process the operands */
    for(;j <= 2; j++){
        strcpy(currWord, findIthWord(word, line, i, " \t\r\n,:#"));
        
        /* If the instruction has one operand, skip the second operand */
        if(numOfOperand == 1){
            (*binary).code[(*binary).codeSize] <<= 2;
            j++;
        }

        /* Check if the current word is a register */
        if(isRegister(currWord) != -1){
            (*binary).code[(*binary).codeSize] += 3;
            registerNum++;

            if(registerNum == 2){   /* If both operands are registers, adjust the line counter */
                lineCuont--;
                (*binary).code[(*binary).codeSize + lineCuont] >>= 2;
            }
            (*binary).code[(*binary).codeSize + lineCuont] += isRegister(currWord);
            if((registerNum == 1 && j == 1))                /* If the first operand is a register, shift it left by 5 bits */
                 (*binary).code[(*binary).codeSize + lineCuont] <<= 5;
            else
                (*binary).code[(*binary).codeSize + lineCuont] <<= 2;              
        }    

        /* Check if the current word is a numeric constant */
        else if(isNumber(currWord)){;
            (*binary).code[(*binary).codeSize + lineCuont] = atoi(currWord);
            (*binary).code[(*binary).codeSize + lineCuont] <<= 2;
        }

        /* Check if the current word is a constant defined elsewhere */
        else if(isConst(tagsTable, currWord)){
            (*binary).code[(*binary).codeSize + lineCuont] += ((struct data*)(findHash(tagsTable ,currWord)->data))->value;
            (*binary).code[(*binary).codeSize + lineCuont] <<= 2;
        }

        /* Check if the current word is a valid label */
        else if(isValidLabel(tagsTable, currWord)){
            (*binary).code[(*binary).codeSize] += 1;
            (*binary).code[(*binary).codeSize + lineCuont] = ((struct data*)(findHash(tagsTable ,currWord)->data))->value;
            (*binary).code[(*binary).codeSize + lineCuont] <<= 2;
            
            /* If the label is external, add it to the external symbols table */
            if(strcmp(((struct data*)(findHash(tagsTable ,currWord)->data))->type, "external") == 0){
                (*binary).code[(*binary).codeSize + lineCuont] += 1;
                strcpy((*binary).ext[(*binary).extSize].name, currWord);
                (*binary).ext[(*binary).extSize++].adrress = (*binary).codeSize + 100 + lineCuont; 
            }    
            else
                (*binary).code[(*binary).codeSize + lineCuont] += 2;
        }

        /* Check if the current word is an indexed label */
        else if(currWord[strlen(currWord) - 1] == ']' && isValidLabel(tagsTable, findIthWord(word, currWord, 1, "["))){
            (*binary).code[(*binary).codeSize] += 2;
            /*binary.code[binary.codeSize] += 1; */
            (*binary).code[(*binary).codeSize + lineCuont] = ((struct data*)(findHash(tagsTable, findIthWord(word, currWord, 1, "["))->data))->value;
            (*binary).code[(*binary).codeSize + lineCuont] <<= 2;

            /* If the label is external, add it to the external symbols table */
            if(strcmp(((struct data*)(findHash(tagsTable ,findIthWord(word, currWord, 1, "["))->data))->type, "external") == 0){
                (*binary).code[(*binary).codeSize + lineCuont] += 1;
                strcpy((*binary).ext[(*binary).extSize].name, findIthWord(word, currWord, 1, "["));
                (*binary).ext[(*binary).extSize++].adrress = (*binary).codeSize + 100 + lineCuont++; 
            }    
            else
                (*binary).code[(*binary).codeSize + lineCuont++] += 2;

            /* Process the index */    
            if(isNumber(findIthWord(word, currWord, 2, " \t[]"))){
                (*binary).code[(*binary).codeSize + lineCuont] = atoi(findIthWord(word, currWord, 2, " \t[]"));
            }
            else    
                (*binary).code[(*binary).codeSize + lineCuont] = ((struct data*)(findHash(tagsTable, findIthWord(word, currWord, 2, " \t[]"))->data))->value;
            (*binary).code[(*binary).codeSize + lineCuont] <<= 2;
        }
        else
            return (*fileIsValid = error(lineNumber, 31));
        
        (*binary).code[(*binary).codeSize] <<= 2;
        lineCuont++;
        i++;
    }
    /*binary.code[binary.codeSize] <<= 2; */    
    return lineCuont;
}

/* Function to check if a label is valid */
bool isValidLabel(hashTable tagsTable, char *symbol){
    return findHash(tagsTable, symbol) && strcmp(((struct data*)(findHash(tagsTable, symbol)->data))->type, "mdefine") != 0; 
}

/* Function to process a data line and enter its binary representation into the binary code */
int enterData(char* line, int lineNumber, bool *fileIsValid, hashTable tagsTable, struct binaryCode *binary){
    char word[MAX_LINE_LENGTH];
    char currWord[MAX_LINE_LENGTH] = {0};
    int i = 1;
    int lineCuont = 0;
    if(isLabel(findIthWord(word, line, i, " \t\r\n,")))
        i += 2;
    else
        i++;
    if(findIthWord(word, line, i, " \t\r\n,") != NULL)
        strcpy(currWord, findIthWord(word, line, i, " \t\r\n,"));        
    while(currWord[0] != '\0'){
        if(findIthWord(word, line, i, " \t\r\n,") == NULL)
            break;
        strcpy(currWord, findIthWord(word, line, i, " \t\r\n,"));

        if(atoi(currWord))            /* If the word is a numeric constant, enter it into the binary code */
            (*binary).data[(*binary).dataSize + lineCuont] = atoi(currWord);
        else if(isConst(tagsTable, currWord))                   /* If the word is a constant defined elsewhere, enter its value into the binary code */
             (*binary).data[(*binary).dataSize + lineCuont] = ((struct data*)(findHash(tagsTable, currWord)->data))->value;
    
        lineCuont++;
        i++;
    }
    return lineCuont;    
}

/* Function to process a string line and enter its binary representation into the binary code */
int enterString(const char* line, int lineNumber, bool *fileIsValid, hashTable tagsTable, struct binaryCode *binary){
    int k = 1;
    int lineCuont = 0;
    char *str = strstr(line, "\"");
    if(str == NULL)
        return 1;

    while(str[k] != '\"'){
        (*binary).data[(*binary).dataSize + lineCuont] = str[k];
        lineCuont++;
        k++;
    }

    (*binary).data[(*binary).dataSize + lineCuont] = 0;
    lineCuont++;
    return lineCuont;     
}

/* Function to print the binary code */
void printbinary(struct binaryCode binary){
    int i = 0;
    int line = 100;
    for(; i < binary.codeSize; i++, line++){
        printf("0%d\t%d\t%d\n", line, binary.code[i], decToBinary(binary.code[i])); 
    }
    i = 0;
    for(; i < binary.dataSize; i++, line++)
        printf("0%d\t%d\t%d\n", line, binary.data[i], decToBinary(binary.data[i]));
}

/* Function to convert a decimal number to binary */
int decToBinary(int num){
    int i = 1, j = 0;
    int binary = 0;
    while(num > 0 && j < 14){
        binary = ((num % 2) * i) + binary;
        num = num / 2;
        i = i * 10;
        j++;
    }
    return binary;
}

/* Function to create the .ob file */
void createOb(const char *fileName, int IC, int DC, struct binaryCode binary){
    char *name = addingSuffix(fileName, ".ob");
    FILE *fileOb = openFile(name, "w");
    int i = 0;
    int lineNumber = 100;
    fprintf(fileOb, "    %d %d\n", IC, DC);
     
    /* Write the instructions to the file */ 
    for(; i < binary.codeSize; i++, lineNumber++){
        char *result = encryptedCode(binary.code[i]);
        fprintf(fileOb, "%04d\t%s\t%d\n", lineNumber, result, binary.code[i]); 
        
        free(result); 
    }

    /* Write the data to the file */
    i = 0;
    for(; i < binary.dataSize; i++, lineNumber++){
        char *result = encryptedCode(binary.data[i]);    
        fprintf(fileOb, "%04d\t%s\t%d\n", lineNumber, result, binary.data[i]);  
        
        free(result);  
    }
    free(name);
    fclose(fileOb);
}

/* Function to encrypt a binary number */
char *encryptedCode(int num){
    char *encryptedWord = (char*)malloc(8 * sizeof(char));
    
    int i = 12, j = 0;
    checkAllocation(encryptedWord);

    for(; i >= 0; i -= 2, j++){
        encryptedWord[j] = encrypted((num >> i) & 3);
    }
    encryptedWord[j] = '\0';
    return encryptedWord;
}

/* Function to encrypt a binary digit */
char encrypted(int num){
    switch(num)
    {
    case 0:
        return '*';
        break;
    case 1: 
        return '#'; 
        break;
    case 2:
        return '%';
        break;
    case 3:
        return '!';
        break;
    }
    return '*';
}

/* Function to create the .ent file */
void createEnt(const char *fileName, hashTable tagsTable){
    char *name = addingSuffix(fileName, ".ent");
    FILE *fileEnt = openFile(name, "w");
    node *head;
    int i = 0;
    for (; i < HASH_SIZE; i++){
        head  = tagsTable[i]; 
        while(head != NULL){
            if(strstr(((struct data*)(head->data))->type, "entry") != NULL)
                fprintf(fileEnt, "%s\t%04d\n", tagsTable[i]->key, ((struct data*)(tagsTable[i]->data))->value); 
            head = head->next;
        }
    }
    free(name);
    fclose(fileEnt);    
}

/* Function to create the .ext file */
void createExt(const char *fileName, struct binaryCode binary){
    char *name = addingSuffix(fileName, ".ext");
    FILE *fileExt = openFile(name, "w");
    int i = 0;
    for (; i < binary.extSize; i++){
        fprintf(fileExt, "%s\t%04d\n", binary.ext[i].name, binary.ext[i].adrress);
    }
    free(name);
    fclose(fileExt);
}
