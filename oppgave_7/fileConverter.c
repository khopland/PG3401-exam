#include "debugger.h"
#include <stdio.h>

void ConvertForLoopToWhileLoop(char c, FILE *fTarget, FILE *fResult);
int checkForForLoop(char c, FILE *fTarget, FILE *fResult);
char whiteSpaceCleanUp(FILE *fTarget, FILE *fResult);
char proxyFgetc(FILE *fTarget, FILE *fResult);

void convertFile(FILE *fTarget, FILE *fResult) {
    char c = 0;
    do {
        c = proxyFgetc(fTarget, fResult);
        if (feof(fTarget)) { // check for end  and break.
            break;
        }

        // need to add the for loops change here
        if (c == 'f') {
            ConvertForLoopToWhileLoop(c, fTarget, fResult);
            continue;
        }

        fputc(c, fResult);
    } while (1);
}

int checkForForLoop(char c, FILE *fTarget, FILE *fResult) {

    if (c != 'f') {
        fprintf(fResult, "%c", c);
        return 1;
    }

    c = proxyFgetc(fTarget, fResult);
    if (c != 'o') {
        fprintf(fResult, "f%c", c);
        return 1;
    }

    c = proxyFgetc(fTarget, fResult);
    if (c != 'r') {
        fprintf(fResult, "fo%c", c);
        return 1;
    }

    c = whiteSpaceCleanUp(fTarget, fResult);

    if (c != '(') {
        fprintf(fResult, "for%c", c);
        return 1;
    }
    return 0;
}

// this function will check for "for loops" and change them to "while loops".
void ConvertForLoopToWhileLoop(char c, FILE *fTarget, FILE *fResult) {
    int indent = 0;
    char incrementor[100];

    // checks if the next 3 characters are "for".
    if (checkForForLoop(c, fTarget, fResult)) {
        return;
    }

    c = whiteSpaceCleanUp(fTarget, fResult);

    // get the first part of the for loop and adds it to the file before the wile loop.
    while (c != ';') {
        fputc(c, fResult);
        c = proxyFgetc(fTarget, fResult);
    }
    fprintf(fResult, ";\n");

    c = whiteSpaceCleanUp(fTarget, fResult);

    // print the start of the while loop
    fprintf(fResult, "while (");

    // get the second part of the for loop and adds it inside for the () of the while loop.
    while (c != ';') {
        fputc(c, fResult);
        c = proxyFgetc(fTarget, fResult);
    }

    // close the while loop
    fprintf(fResult, ")");

    c = whiteSpaceCleanUp(fTarget, fResult);
    // gets the incrementor for the for loop and stores it in a char array.
    int i = 0;
    while (c != ')') {
        incrementor[i] = c;
        i++;
        if (i > 100) {
            error("incrementor is too long");
            return;
        }
        c = proxyFgetc(fTarget, fResult);
    }
    incrementor[i] = '\0';

    c = whiteSpaceCleanUp(fTarget, fResult);

    // handle for loops with body of the for loop and keeps track of the scope indents.
    // if the for loop is a for loop with curly braces. it adds the incrementor to the end of the while loop.
    if (c == '{') {
        indent++;
        fprintf(fResult, " {");

        c = proxyFgetc(fTarget, fResult);
        while (indent > 0) {
            if (c == '}') {
                indent--;
                if (indent == 0) {
                    fprintf(fResult, "%s;\n}", incrementor);
                    break;
                } else {
                    fprintf(fResult, "}");
                }
            } else if (c == '{') {
                indent++;
                fprintf(fResult, "{");
            } else {
                fputc(c, fResult);
            }

            c = proxyFgetc(fTarget, fResult);
        }
        return;
    }

    // handle for loops with no body of the for loop. it adds a empty body and adds the incrementor to the end of the
    // while loop.
    if (c == ';') {
        fprintf(fResult, " { %s; }", incrementor);
        return;
    }

    // handle for loops with body but no curly braces. and adds the incrementor to the end of the while loop.
    fprintf(fResult, " {\n");
    c = proxyFgetc(fTarget, fResult);
    while (c != ';') {
        fputc(c, fResult);
        c = proxyFgetc(fTarget, fResult);
    }
    fprintf(fResult, ";\n%s;\n}", incrementor);
    return;
}

// remove white space.
char whiteSpaceCleanUp(FILE *fTarget, FILE *fResult) {
    char c;
    c = proxyFgetc(fTarget, fResult);
    while (c == ' ' || c == '\n' || c == '\r' || c == '\t') {
        c = proxyFgetc(fTarget, fResult);
    }
    return c;
}

// proxy function to handle tabs and replace them with 3 spaces.
char proxyFgetc(FILE *fTarget, FILE *fResult) {
    char c = fgetc(fTarget);

    while (c == '\t') {
        fprintf(fResult, "   ");
        c = fgetc(fTarget);
    }
    return c;
}