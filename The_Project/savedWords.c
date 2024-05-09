#include "savedWords.h"

/* Check if the word is a saved word. */
int savedWord(const char *word) {
    char *savedWords[] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", 
                          "jmp", "bne", "red", "prn", "jsr", "rts", "hlt",
                          "data", "string", "entry", "extern", "define", "mcr", "endmcr",
                          "r1", "r2", "r3", "r4", "r5", "r6", "r7"};
    int numWords = sizeof(savedWords) / sizeof(savedWords[0]); /* Calculate the number of elements in the array */
    int i = 0;
    for (; i < numWords; i++) {
        if (strcmp(word, savedWords[i]) == 0) {
            return i + 1; /* Return i+1 to distinguish between match at index 0 and non-match */
        }
    }
    return 0;
}

/* Check if the line is a comment. */
bool isComment(char *line){

    if(line[0] == ';')
        return true;
    return false;
}

/* Check if the line is too long. */
bool isTooLong(char *line, bool *fileIsValid, int lineNumber){
    if(strchr(line,'\n') == NULL && strlen(line) >= MAX_LINE_LENGTH - 1)
        return (*fileIsValid = error(lineNumber, 28));
    return true;        
}

/* Check if the line contains the .define directive. */
bool isDefine(char *line){
    char word[MAX_LINE_LENGTH];
    return strcmp(findIthWord(word, line, 1, " \t\r\n"), ".define") == 0;
}

/* Check the syntax of the .define directive. */
bool checkDefine(char *line, int IC, int DC, bool *fileIsValid, int lineNumber, hashTable tagsTable, hashTable macroTable){
    char word[MAX_LINE_LENGTH];
    return checkSymbol(findIthWord(word, line, 2, " \t\r\n="), ".define", false, IC, DC,  fileIsValid, lineNumber, tagsTable, macroTable) && checkEqual(line, fileIsValid, lineNumber);
}

/* Check if the .define directive includes valid '=' symbol. */
bool checkEqual(char *line, bool *fileIsValid, int lineNumber){
    char word[MAX_LINE_LENGTH];
    char word1[MAX_LINE_LENGTH];
    char word3[MAX_LINE_LENGTH];

    if (findIthWord(word, line, 4, " \t\r\n=") != NULL)
       *fileIsValid = error(lineNumber, 8); 

    if (strstr(line, "=") == NULL)
       *fileIsValid = error(lineNumber, 9);  

    
    if (strstr(strstr(line, "=") + 1, "=") != NULL)
        *fileIsValid = error(lineNumber, 10); 
    
    if (findIthWord(word, line, 3, " \t\r\n=") == NULL)
        *fileIsValid = error(lineNumber,11); 

    strcpy(word3, findIthWord(word, line, 3, " \t\r\n="));
    if (!isNumber(word3))
        *fileIsValid = error(lineNumber, 12); 

    strcpy(word1, findIthWord(word, line, 1, "="));
    if(findIthWord(word, word1, 2, " \t") == NULL)
        *fileIsValid = error(lineNumber, 13);

    if(findIthWord(word, line, 2, "\r\n=") == NULL)
        *fileIsValid = error(lineNumber, 13);       

    return true;
}

/* Check if the string is a number. */
bool isNumber(char *str){
    int i = 1;
    if(!isdigit(str[0]) && str[0] != '-' && str[0] != '+')
        return false;
    while(str[i] != '\0'){
        if(!isdigit(str[i]))
            return false;
        i++;
    }
    return true;
}

/* Check if the line contains the .extern directive. */
bool isExtern(char *line){
    char word[MAX_LINE_LENGTH];
    if(strcmp(findIthWord(word, line, 1, " \t\r\n,"), ".extern") == 0)
        return true;
    return false;    
}

/* Check if the line contains the .entry directive. */
bool isEntry(char *line){
    char word[MAX_LINE_LENGTH];
    if(strcmp(findIthWord(word, line, 1, " \t\r\n,"), ".entry") == 0)
        return true;
    return false;    
}

/* Check the syntax of the .entry directive. */
int checkEntry(char *line, bool *fileIsValid, int lineNumber, hashTable tagsTable){
    char word[MAX_LINE_LENGTH];
    char symbol[MAX_LINE_LENGTH];
    
    if(findIthWord(word, line, 3, " \t\r\n") != NULL)
        return (*fileIsValid = error(lineNumber, 29));
    
    strcpy(symbol, findIthWord(word, line, 2, " \t\r\n:"));
    if(symbol == NULL || !findHash(tagsTable, symbol))
        return (*fileIsValid = error(lineNumber, 30));
    strcat(((struct data*)(findHash(tagsTable, symbol)->data))->type, "entry");
    return 1;
}


/* Check if the symbol is valid. */
bool checkSymbol(char *symbol, char *cmd, bool label, int IC, int DC, bool *fileIsValid, int lineNumber, hashTable tagsTable, hashTable macroTable){
    char symbol1[MAX_LINE_LENGTH];

    if(symbol == NULL)
        return (*fileIsValid = error(lineNumber, 14)); 

    strcpy(symbol1, symbol);
    if(strlen(symbol1) > MAX_TAG_LENGTH)                          /* Check the symbol length */
        return (*fileIsValid = error(lineNumber, 15)); 

    else if(!isalpha(symbol1[0]))                               /*   Check that the symbol starts with a letter         */
        return (*fileIsValid = error(lineNumber, 16)); 

    else if(findHash(tagsTable, symbol1) != NULL || findHash(macroTable, symbol1) != NULL || savedWord(symbol1))   /*   Check if the word is a served word. */
        return (*fileIsValid = error(lineNumber, 17)); 
      
    return checkCmd(symbol1, cmd, label, IC, DC, fileIsValid, lineNumber, tagsTable);
}

/* Check if the command is valid. */
bool checkCmd(char *symbol1, char *cmd, bool label, int IC, int DC, bool *fileIsValid, int lineNumber, hashTable tagsTable){
    if(cmd){
        unsigned int address= 0;
        char *type = NULL;
        int i = 0;        
        if(instructionWord(cmd) != -1){
            address = IC;
            type = "code";
        }
        else if((i = promptWord(cmd))){
            
            if(i == 5 &&  label)
                return (*fileIsValid = error(lineNumber, 18)); 

            if(i == 5){
               return true;
            }

            if(i == 3 || (i == 4 && label)){
               printf("Warning: line %d: label before entry/extern.\n", lineNumber);
               return true; 
            }
            
            if(i == 4){
                address = 0;
                type = "external";
            }

            if(i == 1 || i == 2){
                address = DC;
                type = "data";
            }        
        }
        insertHash(tagsTable, createNode(NULL, symbol1, createData(type , address)));
        return true;
    }
    return true;   /*       line only with a label.   it's valid?           */  
}

/* Check if the line is a label. */
bool isLabel(char *line){
    char word[MAX_LINE_LENGTH];
    char word1[MAX_LINE_LENGTH];
    strcpy(word1, findIthWord(word, line, 1, " \t\r\n"));
    return word1[strlen(word1) -1] == ':';
}
/* Check if the line contains the .string directive. */
bool isString(char *line, int i){
    char word[MAX_LINE_LENGTH];
    return strcmp(findIthWord(word, line, i, " \t\r\n"), ".string") == 0;
}

/* Check the syntax of the .string directive. */
bool checkString(char *line, int i, bool *fileIsValid, int lineNumber, hashTable tagsTable, hashTable macroTable){
    int j = 1;
    char *str = strstr(line, "\"");
    if(str == NULL)
        return (*fileIsValid = error(lineNumber, 19));

    while(str[j] != '\0'){
        
        if(str[j] == '\n' || str[j] == '\r' || j == MAX_LINE_LENGTH)
            break;
        if(!isprint(str[j]))
            return (*fileIsValid = error(lineNumber, 19));     
        j++;
    }

    while(str[j] != '\"'){
        if((str[j] != ' ' && str[j] != '\t' && str[j] != '\n' && str[j] != '\r') || j == 1)
            return (*fileIsValid = error(lineNumber, 19)); 
        j--;
    }
    return true;
}

/* Check if the line contains the .data directive. */
bool isData(char *line, int i){
    char word[MAX_LINE_LENGTH];
    return strcmp(findIthWord(word, line, i, " \t\r\n"), ".data") == 0;
}

/* Check the syntax of the .data directive. */
bool checkData(char *line, int i, bool *fileIsValid, int lineNumber, hashTable tagsTable, hashTable macroTable){
    int j = 0;
    bool commaExpected = false;
    char word[MAX_LINE_LENGTH];
    char *token;

    while(line[j] != '.'){
        j++;
    }
    j += 5;
    while (line[j] != '\0') {
        if ((isalnum(line[j]) || line[j] == '-' || line[j] == '+') && !commaExpected){
            commaExpected = true;
            while (isalnum(line[j]) || line[j] == '-' || line[j] == '+')
                j++;    
        } 
        else if (line[j] == ' ' || line[j] == '\t'){
            while (line[j] == ' ' || line[j] == '\t')
                j++;                                     /* Ignore spaces between words/numbers and commas */ 
        } 
        else if (line[j] == ',' && commaExpected){
            commaExpected = false;
            j++;
        }
        else if(line[j] == '\n' || line[j] == '\r'){
            commaExpected = false;
            break;
        }        
        else
            return (*fileIsValid = error(lineNumber, 21));
    }

    if (commaExpected && line[j] != '\0')
        return (*fileIsValid = error(lineNumber, 22));

    while((token = findIthWord(word, line, i++, " \t\r\n,"))){
        if(!isNumber(token) && !isConst(tagsTable, token))
            return (*fileIsValid = error(lineNumber, 23));
    }
    return true;
}

/* Check if the word is a constant. */
bool isConst(hashTable tagsTable, char *symbol){
    return findHash(tagsTable ,symbol) != NULL && strcmp(((struct data*)(findHash(tagsTable ,symbol)->data))->type, "mdefine") == 0; 
}

/* Count the number of elements in a data line. */
int sizeOfData(char line[]){
    int i = 0;
    int count = 0;
    for(; i < strlen(line); i++)
        if(line[i] == ',')
            count++;
    return count+1;
}

/* Check if the word is an instruction word. */
int instructionWord(const char *word){
    char *instructionWord[] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", 
                               "dec", "jmp", "bne", "red","prn", "jsr", "rts", "hlt"};
    int i = 0;
     int numWords = sizeof(instructionWord) / sizeof(instructionWord[0]);
    
    for (; i < numWords; i++){

        if(strcmp(word, instructionWord[i]) == 0)
            return i;
    }
    return EOF;
}

/* Check the syntax of an instruction. */
bool checkInstruction(char *line, int i, int cmdType, int *ICCuont, bool *fileIsValid, int lineNumber, hashTable table){
    char word[MAX_LINE_LENGTH];
    char word1[MAX_LINE_LENGTH];
    char word2[MAX_LINE_LENGTH];

    char *param1;
    char *param2;

    if(findIthWord(word, line, i+3, " \t\r\n,") != NULL)
        return (*fileIsValid = error(lineNumber, 27));

    param1 = findIthWord(word1, line, i+1, " \t\r\n,");
    param2 = findIthWord(word2, line, i+2, " \t\r\n,");
 
    if(typeOfParam(param1, table) == -1 || typeOfParam(param2, table) == -1)
        return (*fileIsValid = error(lineNumber, 23));

    if(!chekNumParams(cmdType, param1, param2))
        return (*fileIsValid = error(lineNumber, 24));

    if(!checkParams(cmdType, param1, param2, table))
        return (*fileIsValid = error(lineNumber, 25));

    if(!checkComma(line, i, cmdType, param2))
        return (*fileIsValid = error(lineNumber, 26));

    *ICCuont = sizeOfInstruction(typeOfParam(param1, table), typeOfParam(param2, table));
    return true;
}

/* Check if the line contains the required comma for the instruction type. */
bool checkComma(char *line, int i, int cmdType, char *param2){
    char word[MAX_LINE_LENGTH];
    int j = 0, cuont = 0;
    char *word1;
    for(; line[j] != '\0'; j++){
        if(line[j] == ',')
            cuont++;
    }
    if((0 < cuont && 3 < cmdType && cmdType != 6) || 1 < cuont)
        return false;
    if(cuont == 0 && 3 < cmdType && cmdType != 6)
        return true;
    if(cuont == 0)
        return false;
    if(findIthWord(word, line, 2, ",") == NULL)
        return false;  

    word1 = findIthWord(word, line, 2, ",");
    if(findIthWord(word, word1, 1, " \t\r\n") == NULL)
        return false;
    if(strcmp(findIthWord(word, word1, 1, " \t\r\n"), param2) == 0)
        return true;
    return false;    
}

/* Calculate the size of an instruction. */
int sizeOfInstruction(int typeOfParam1, int typeOfParam2){
    int ICCuont = 1;
    if(0 <= typeOfParam1)
        ICCuont++;
    if(0 <= typeOfParam2)
        ICCuont++;
    if(typeOfParam1 == 3 && typeOfParam2 == 3)
        ICCuont--;
    if(typeOfParam1 == 2)
        ICCuont++;
    if(typeOfParam2 == 2)
        ICCuont++;
       
    return ICCuont;             
}

/* Check if the parameters match the command type. */
bool checkParams(int cmdType, char *param1, char *param2, hashTable table){
    int typeParam1 = typeOfParam(param1, table);
    int typeParam2 = typeOfParam(param2, table);  
    
    if(cmdType == 14 || cmdType == 15);
    else if((0 <= cmdType && cmdType <= 3) || cmdType == 12);
    else if(typeParam1 > 0 && ((cmdType == 4 || cmdType == 5 || cmdType == 7 || cmdType == 8 || cmdType == 11)));    
    else if(cmdType == 6 && (typeParam1 == 1 || typeParam1 == 2));
    else if((cmdType == 9 || cmdType == 10 || cmdType == 13) && (typeParam1 == 1 || typeParam1 == 3));
    else
        return false;

    if(cmdType == 1 || cmdType == 4 || cmdType == 5 || 6 < cmdType);
    else if((cmdType == 0 || cmdType == 2 || cmdType == 3 || cmdType == 6) && 0 < typeParam2);
    else 
        return false;
    
    return true;        
}

/* Check if the number of parameters is correct for the command. */
bool chekNumParams(int cmdType, char *param1, char *param2){
    return numOfOperands(cmdType) == ((param1) ? 1 : 0) + ((param2) ? 1 : 0);
}

/* Check the type of parameter. */
int typeOfParam(char param[], hashTable table){
    char word[MAX_LINE_LENGTH];
    
    if(param == NULL)
        return -2;

    if(isRegister(param) != -1)
        return 3;

    else if(param[0] == '#'){
    char *number = findIthWord(word, param, 1, " \t\r\n#");
    if(isNumber(number) ||((isConst(table, number) && strcmp(((struct data*) ((findHash(table, number)->data)))->type, "mdefine") == 0))) 
        return 0;
    return -1;    
    }

    else if(isalpha(param[0]) && strstr(param, "[") == NULL){
        return 1;
    }

    else if(isalpha(param[0]) && strstr(param, "[") != NULL && strstr(param, "]") != NULL){
    char *number = findIthWord(word, param, 2, " \t\r\n[]");    
    if(isNumber(number) ||((isConst(table, number) && strcmp(((struct data*) ((findHash(table, number)->data)))->type, "mdefine") == 0 && checkBraket(param)))) 
        return 2;

    return -1;    
    }
    
    else
        return -1;
}

bool checkBraket(char *param){
    char *define = strstr(param, "[");
    int i = 0, count = 0;
    while(define[i] != '\0'){
        if(define[i] == '[' || define[i] == ']')
            count++;
        i++;    
    }
    return define[0] == '[' && define[strlen(define)-1] == ']' && count == 2; 
}

int numOfOperands(int i){
    int numOfOperand[] = {2, 2, 2, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 0, 0};
    return numOfOperand[i];
}

/* Check if the word is a prompt word. */
int promptWord(const char *word) {
    char *prompts[] = {".data", ".string", ".entry", ".extern", ".define"};
    int numPrompts = sizeof(prompts) / sizeof(prompts[0]);
    int i = 0;
    for (; i < numPrompts; i++) {
        if (strcmp(word, prompts[i]) == 0)            
            return i + 1;

    }
    return 0;
}

/* Check if the word is a register. */
int isRegister(const char *word){
    char *registers[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};
    int numRgisters = sizeof(registers) / sizeof(registers[0]);
    int i = 0;
    for (; i < numRgisters; i++){
        if(strcmp(word, registers[i]) == 0)
            return i;
    }
    return -1;
}

/* Add the value of IC to DC. */
void addICtoDC(hashTable table, int IC){
    int i = 0;
    for (; i < HASH_SIZE; i++){
        node *curr = table[i];
        while(curr != NULL){
            if(strcmp(((struct data*)(curr->data))->type, "data") == 0)
                 ((struct data*)(curr->data))->value += IC;
        curr = curr->next;         
        }
    }
}

/* Check if the word is an illegal word. */
bool isIllegalWord(char* line, int i){
    char word[MAX_LINE_LENGTH];
    return !isLabel(line) && !isDefine(line) && !isString(line, i) && !isData(line, i) && !isExtern(line) && !(instructionWord(findIthWord(word, line, 1, " \t\r\n,")) != EOF) && !isEntry(line);
}

