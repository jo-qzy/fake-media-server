//
// Created by BoringWednesday on 2021/5/16.
//

#include "socket_util.h"
#include <unistd.h>
#include <fcntl.h>

int create_socket(int domain, int type, int protocol)
{
    int sock = socket(domain, type, protocol);
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

    return sock;
}

int close_socket(int sock)
{
    return close(sock);
}

int accept_connection(int sock, struct sockaddr *addr, socklen_t *addr_len)
{
    return accept(sock, addr, addr_len);
}
