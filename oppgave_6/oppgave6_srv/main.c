#include "debugger.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

char *readFile(char *name);
void Send404(int socket);
void Send500(int socket);
void Send405(int socket);
void send200(int socket, char *content);

int main(int argc, char *argv[]) {

    int socket_desc, new_socket, c;
    struct sockaddr_in server, client;
    char message[2000] = {0};

    int iPort = atoi("8080");

    // Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        error("no socket generated");
        puts("Could not create socket");
    }

    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = htons(iPort);

    // Connect to remote server
    if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0) {
        error("bind failed");
        puts("bind failed");
        return 1;
    }
    puts("bind done");
    listen(socket_desc, 5);

    puts("Waiting for incoming connections...");

    c = sizeof(struct sockaddr_in);
    while ((new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c))) {
        puts("Connection accepted");

        if (read(new_socket, message, 2000 - 1) < 0) {
            puts("failed to read");
            close(new_socket);
            continue;
        };

        char *method = strtok(message, " ");

        if (method == NULL) {
            puts("cant parse message, closing the conenction");
            Send500(new_socket);
            close(new_socket);
            continue;
        }

        if (strncmp(method, "GET", 3) != 0) {
            puts("method not GET, closing the conenction");
            Send405(new_socket);
            close(new_socket);
            continue;
        }

        char *fileName = strtok(NULL, " ");
        char *fileContent = readFile(fileName + 1); // +1 for trimming the /

        if (fileContent == NULL) {
            puts("fileContent is NULL, closing the conenction");
            Send404(new_socket);
            close(new_socket);
            continue;
        }
        send200(new_socket, fileContent);

        // cleanup
        free(fileContent);
        close(new_socket);
    }
    return 0;
}

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

char *readFile(char *name) {
    FILE *fReader = NULL;
    int lSize, lBytesRead;
    char *byBuffer = NULL;

    fReader = fopen(name, "rb");

    if (fReader == NULL) {
        puts("fReader is NULL closing conection");
        return NULL;
    }

    if (fseek(fReader, 0, SEEK_END) == 0) {
        lSize = ftell(fReader);
        rewind(fReader);
        byBuffer = malloc(lSize);
        if (byBuffer != NULL) {
            while (!feof(fReader)) {
                lBytesRead = fread(byBuffer, sizeof(char), lSize, fReader);
                if (lBytesRead == lSize) {
                    fclose(fReader);
                    return byBuffer;
                }
            }
        }
    }
    fclose(fReader);
    return NULL;
}