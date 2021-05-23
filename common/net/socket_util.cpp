//
// Created by BoringWednesday on 2021/5/16.
//

#include "socket_util.h"
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>

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

int bind_and_listen(int sock, const char *addr, int port, int backlog)
{
    sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = inet_addr(addr);

    if (bind(sock, (sockaddr *) &address, sizeof address) != 0) {
        return -1;
    }

    if (listen(sock, backlog) != 0) {
        return -1;
    }

    return 0;
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
