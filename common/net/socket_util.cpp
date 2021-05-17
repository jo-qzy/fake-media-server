//
// Created by BoringWednesday on 2021/5/16.
//

#include "socket_util.h"
#include <unistd.h>
#include <fcntl.h>

int create_tcp_socket()
{
    return socket(AF_INET, SOCK_STREAM, 0);
}

int create_tcp_socket_nonblock()
{
#if defined(__APPLE__)
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    set_nonblock(sock);
#elif defined(__linux__)
    int sock = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
#endif

    return sock;
}

int accept_connection(int sock, struct sockaddr *addr, socklen_t *addr_len)
{
    return accept(sock, addr, addr_len);
}

int close_socket(int sock)
{
    return close(sock);
}

int set_nonblock(int sock)
{
    int flags = fcntl(sock, F_GETFL, 0);
    if (flags < 0) {
        close_socket(sock);

        return -1;
    }

    flags |= O_NONBLOCK;
    if (fcntl(sock, F_SETFL, flags) < 0) {
        close_socket(sock);

        return -1;
    }

    return 0;
}
