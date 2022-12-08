#include "debugger.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _MYHTTP {
    int iHttpCode;
    float fHttpVersion;
    int iContentLength;
    bool bIsSuccess;
    char szServer[16];
    char szContentType[16];
} MYHTTP;

MYHTTP *ProcessHttpHeader(char *pszHttp);

int main(void) {

    MYHTTP *res = ProcessHttpHeader("HTTP/1.1 200 OK \n Server: Apache \n Content-Type: text/json\n Content-Length: 155");

    printf("http version: %.1f\n", res->fHttpVersion);
    printf("http code: %i\n", res->iHttpCode);
    printf("Content Length: %i\n", res->iContentLength);
    printf("Is Success: %i\n", res->bIsSuccess);
    printf("server : %s\n", res->szServer);
    printf("ContentType : %s\n", res->szContentType);
    return 0;
}
MYHTTP *ProcessHttpHeader(char *pszHttp) {
    char *pszPtr;
    MYHTTP *pHttp =
        (MYHTTP *)malloc(sizeof(MYHTTP)); // allocates 8 bytes (pointer), not the 44 bytes as needed for the struct
    debug("length of pHttp: %i", sizeof(*pHttp));
    if (!pHttp)
        return NULL;
    memset(pHttp, 0, sizeof(MYHTTP)); // sets 44 bytes to 0, but pHttp is only 8 bytes long

    pHttp->fHttpVersion = atof( pszHttp + strlen("HTTP/"));

    pHttp->iHttpCode = atoi(pszHttp + strlen("HTTP/1.x "));
    if (pHttp->iHttpCode == 200) {
        pHttp->bIsSuccess = true;
    }
    pszPtr = strstr(pszHttp, "Server");
    if (pszPtr) {
        pszPtr += 6;
        while (!isalpha(pszPtr[0]))
            pszPtr++;
        int len = strcspn(pszPtr, "\n");
        if (len >15)
            len = 15;
        
        strncpy(pHttp->szServer, pszPtr, len); // pHttp->szServer is not allocated correctly
    }

    pszPtr = strstr(pszHttp, "Content-Type"); 
    if (pszPtr) {
        pszPtr += 12;
        while (!isalpha(pszPtr[0]))
            pszPtr++;
        int len = strcspn(pszPtr, "\n");
        if (len >15)
            len = 15;
        
        strncpy(pHttp->szContentType, pszPtr,len); // pHttp->szContentType is not allocated correctly
    }
    pszPtr = strstr(pszHttp, "Content-Length");
    if (pszPtr) {
        pszPtr += 14;
        while (!isdigit(pszPtr[0]))
            pszPtr++;
        pHttp->iContentLength = '0' + atoi(pszPtr);
    }
    return pHttp;
}