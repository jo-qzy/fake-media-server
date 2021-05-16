//
// Created by BoringWednesday on 2021/5/16.
//

#ifndef FAKE_MEDIA_SERVER_SOCK_UTIL_H
#define FAKE_MEDIA_SERVER_SOCK_UTIL_H

#include <sys/socket.h>

int create_socket(int domain, int type, int protocol);
int close_socket(int sock);
int accept_connection(int sock, struct sockaddr *addr, socklen_t *addr_len);


#endif //FAKE_MEDIA_SERVER_SOCK_UTIL_H
