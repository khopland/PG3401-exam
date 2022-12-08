#include "debugger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    FILE *fTarget = NULL; // file for reading
    FILE *fResult = NULL; // file for writing
    char c; 
    char *targetFile = "data.txt"; // name of target file
    char *ResultFile = "data_result.txt";  // name of result file
    
    fTarget = fopen(targetFile, "r"); //  opens read file stream.
    if (fTarget == NULL) {
        error("failed to create fTarget");
        return 1;
    }
    fResult = fopen(ResultFile, "w"); //  opens write file stream.
    if (fResult == NULL) {
        error("failed to create fResult");
        return 1;
    }
    // read each char and print it as hex
    do {
        c = fgetc(fTarget);
        fprintf(fResult, "%02X", c);
    } while (!feof(fTarget));

    // clean up / close file readers.
    fclose(fTarget);
    fTarget = NULL;
    fclose(fResult);
    fResult = NULL;
    return 0;
}