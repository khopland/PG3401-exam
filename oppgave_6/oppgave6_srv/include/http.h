#ifndef __HTTP_H__
#define __HTTP_H__
void Send404(int socket);
void Send500(int socket);
void Send405(int socket);
void send200(int socket, char *content);
#endif