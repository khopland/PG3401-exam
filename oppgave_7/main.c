#include "debugger.h"
#include "fileConverter.h"
#include <stdlib.h>
#include <string.h>

int strEndsWith(char *str, char *suffix);
char *removeFileExtension(char *myStr);

int main(int argc, char *argv[]) {
    FILE *fTarget = NULL;    // file for reading
    FILE *fResult = NULL;    // file for writing
    char *targetFile = NULL; // name of target file

    // get file name from user
    if (argc != 2) {
        printf("Usage: %s <target file>\n", argv[0]);
        return 1;
    }

    // get target file name
    targetFile = argv[1];
    if (targetFile == NULL) {
        error("failed to get target file");
        return 1;
    }

    // check if target file name is too short
    if (strlen(targetFile) < 1) {
        error("target file name is too short");
        return 1;
    }

    // check if target file is a .c file
    if (!strEndsWith(targetFile, ".c")) {
        error("target file is not a .c file");
        return 1;
    }

    // create file name without extension
    char *targetName = removeFileExtension(targetFile);
    if (targetName == NULL) {
        error("failed to remove file extension");
        return 1;
    }

    // create file name for result file
    char *ResultFile = (char *)malloc(strlen(targetName) + 13);
    if (ResultFile == NULL) {
        error("failed to allocate memory for ResultFile");
        return 1;
    }
    if (sprintf(ResultFile, "%s_beautified.c", targetName) < 0) {
        error("failed to create ResultFile");
        return 1;
    }

    // free memory for targetName
    free(targetName);
    targetName = NULL;

    // opens read file stream.
    fTarget = fopen(targetFile, "r");
    if (fTarget == NULL) {
        error("failed to create fTarget");
        return 1;
    }

    //  opens write file stream.
    fResult = fopen(ResultFile, "w");
    if (fResult == NULL) {
        error("failed to create fResult");
        return 1;
    }

    // do logic here
    convertFile(fTarget, fResult);

    // clean up / close file readers.
    fclose(fTarget);
    fTarget = NULL;
    fclose(fResult);
    fResult = NULL;
    return 0;
}

// check if string ends with suffix
int strEndsWith(char *str, char *suffix) {
    if (!str || !suffix)
        return 0;

    int strLength = strlen(str);
    size_t suffixLength = strlen(suffix);

    if (suffixLength > strLength)
        return 0;

    return strncmp(str + strLength - suffixLength, suffix, suffixLength) == 0;
}

// remove file extension from string
char *removeFileExtension(char *myStr) {
    char *retStr;
    char *lastExt;

    if (myStr == NULL)
        return NULL;

    if ((retStr = malloc(strlen(myStr) + 1)) == NULL)
        return NULL;

    strcpy(retStr, myStr);
    lastExt = strrchr(retStr, '.');

    if (lastExt != NULL)
        *lastExt = '\0';

    return retStr;
}