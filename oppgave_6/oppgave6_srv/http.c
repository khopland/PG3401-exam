#include "debugger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void send200(int socket, char *content) {

    char *response = malloc(strlen(content) + 200);

    sprintf(response,
            "HTTP/1.0 200 OK\r\n Content-Type: text/plain\r\n Content-Length: %li \r\nConnection: close\r\n\r\n%s",
            strlen(content), content);

    if (write(socket, response, strlen(response)) < 0) {
        puts("failed to send 200 with content");
    } else {
        puts("writing to the client and closing");
    }
    // cleanup
    free(response);
}

void Send404(int socket) {
    char *message = "HTTP/1.0 404 NotFound \r\nContent-Length: 0 \r\nConnection: close\r\n\r\n";
    if (write(socket, message, strlen(message)) < 0) {
        puts("failed to send 404");
    } else {
        puts("sent 404");
    }
}

void Send500(int socket) {
    char *message = "HTTP/1.0 500 Internal Server Error \r\nContent-Length: 0 \r\nConnection: close\r\n\r\n";
    if (write(socket, message, strlen(message)) < 0) {
        puts("failed to send 500");
    } else {
        puts("sent 500");
    }
}

void Send405(int socket) {
    char *message = "HTTP/1.0 405 Method Not Allowed \r\nContent-Length: 0 \r\nConnection: close\r\n\r\n";
    if (write(socket, message, strlen(message)) < 0) {
        puts("failed to send 405");
    } else {
        puts("sent 405");
    }
}