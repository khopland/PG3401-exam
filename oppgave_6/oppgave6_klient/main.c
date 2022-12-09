#include "debugger.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

int GetArgs(int *port, int argc, char *argv[]);

int main(int argc, char *argv[]) {

    int iSocket;
    struct sockaddr_in server;
    char strBuf[2000] = {0};

    int port = 8080;
    
    if (argc != 2) {
        return 1;
    }
    char * filename = argv[1];

    iSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (iSocket == -1) {
        error("no socket generated");
        puts("Could not create socket");
        return 1;
    }

    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    // Connect to remote server
    if (connect(iSocket, (struct sockaddr *)&server, sizeof(server)) < 0) {
        puts("connect error");
        return 1;
    }

    puts("Connected");
    sprintf(strBuf,"GET /%s HTTP/1.0\r\n",filename);
    printf("%s",strBuf);
    if (send(iSocket, strBuf, strlen(strBuf), 0) < 0) {
        puts("send failed");
    }
    memset(strBuf, 0, sizeof(strBuf));
    if (recv(iSocket, strBuf, 2000, 0) < 0) {
        puts("recv failed");
    }
    puts(strBuf);
    memset(strBuf, 0, sizeof(strBuf));

    close(iSocket);
    return 0;
}