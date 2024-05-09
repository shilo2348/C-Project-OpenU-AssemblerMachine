#include <stdio.h>
#include <stdlib.h>
#include "handleMain.h"

/*
* The program accepts names of files (one or more) containing code in assembly language, interprets the macros and produces 3 files.
* A file with the extension ob- contains the encrypted base 4 machine language code.
* A file with the extension ent- contains the symbols that are defined as entries.
* A file with the extension ext- contains the symbols that are defined as extern.
*
*The instructions for coding and the rules for the correctness of the input
* are in the course booklet, The Open University, Systems Programming Laboratory, 2024a.
*
* The main sends one file to handelMain each time.
*/

int main(int argc, char *argv[]) {
    int i = 1;
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <inputAssemblyFile>\n", argv[0]);
        return EXIT_FAILURE;
    }

    /* Iterate through command line arguments */
    while(--argc){
        handleMain(argv[i]);
        i++;
    }
    return 0;
}
