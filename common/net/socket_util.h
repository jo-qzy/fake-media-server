//
// Created by BoringWednesday on 2021/5/16.
//

#ifndef FAKE_MEDIA_SERVER_SOCK_UTIL_H
#define FAKE_MEDIA_SERVER_SOCK_UTIL_H

#include <sys/socket.h>

int create_tcp_socket();
int create_tcp_socket_nonblock();
int bind_and_listen(int sock, const char *addr, int port, int backlog);
int accept_connection(int sock, struct sockaddr *addr, socklen_t *addr_len);
int close_socket(int sock);

int set_nonblock(int sock);


#endif //FAKE_MEDIA_SERVER_SOCK_UTIL_H
