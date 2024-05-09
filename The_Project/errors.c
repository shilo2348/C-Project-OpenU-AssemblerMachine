#include "errors.h"


Bool error(int lineNumber, int errorNumber){
    printf("error: line %d: ", lineNumber);
    switch(errorNumber)
    {
    case 1:
        printf("missing macro name or two names for one macro.\n");
        break;
    case 2:
        printf("macro name must to begin with a letter.\n");
        break;
    case 3:
        printf("You must not define macro with the name of saved word.\n");
        break;
    case 4:
        printf("You must not define two macros with the same name.\n");
        break;
    case 5:
        printf("You must not write anything after endmcr.\n");
        break;
    case 6:
        printf("You must not write anything after the macro name.\n");
        break;
    case 7:
        printf("The symbol is unknown.\n");
        break;
    case 8:
        printf("too many opernds.\n");
        break;
    case 9:
        printf("expected '=' in define.\n");
        break;
    case 10:
        printf("too many '='.\n");
        break;
    case 11:
        printf("expected number after '='.\n");
        break;
    case 12:
        printf("expected number after '='.\n");
        break;
    case 13:
        printf("the '=' out of place.\n");
        break;
    case 14:
        printf("expected symbol after define.\n");
        break;
    case 15:
        printf("The symbol too long.\n");
        break;
    case 16:
        printf("The symbol must to begin with a letter.\n");
        break;
    case 17:
        printf("The symbol is saved word.\n");
        break;
    case 18:
        printf("Setting define after a label is not allowed.\n");
        break;
    case 19:
        printf("Expected \" at the beginning and at the end.\n");
        break;
    case 20:
        printf("too many operands.\n");
        break;
    case 21:
        printf("problem with the \" , \" .\n");
        break;
    case 22:
        printf("too many , .\n");
        break;
    case 23:
        printf("invalid parameter.\n");
        break;
    case 24:
        printf("The number of operands does not match.\n");
        break;
    case 25:
        printf("The operands does not match.\n");
        break;
    case 26:
        printf("problem with the commas.\n");
        break;
    case 27:
        printf("too many operands.\n");
        break;
    case 28:
        printf("The line contains more than 80 characters.\n");
        break;
    case 29:
        printf("expected one label after entry.\n");
        break;
     case 30:
        printf("entry symbol not fuond in this file.\n");
        break;
    case 31:
        printf("Unrecognized symbol.\n");
        break;
/*    case 26:
        printf("expected '=' in define.\n");
        break;
    case 26:
        printf("expected '=' in define.\n");
        break;                            
*/

    default: ;
        break;
    }
    return FALSE;
}
