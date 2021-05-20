//
// Created by BoringWednesday on 2021/5/18.
//

#include <echo_client.h>
#include <echo_server.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

int echo_send(void *param, const void *packet, size_t bytes)
{
    int sock = *(int *) param;

    return (int) send(sock, packet, bytes, 0);
}

int echo_onpacket(void *param, const void *data, size_t bytes, uint32_t sequence)
{
    printf("receive message:\n");
    printf("sequence: %d message: %s\n", sequence, (const char *) data);

    return 0;
}

int echo_server_test(int sock)
{
    struct echo_server_handler_t handler;
    struct echo_server_t           *echo_server;
    char buffer[50];
    int read_size;

    handler.send = echo_send;

    echo_server = echo_server_create(&sock, &handler);

    read_size = (int) recv(sock, buffer, 50, 0);
    echo_server_input(echo_server, buffer, read_size);
    read_size = (int) recv(sock, buffer, 50, 0);
    echo_server_input(echo_server, buffer, read_size);

    return 0;
}

int echo_client_test(int sock)
{
    struct echo_client_t           *echo_client;
    struct echo_client_handler_t    handler;
    char buffer[50];
    int read_size;

    handler.send = echo_send;
    handler.onpacket = echo_onpacket;

    echo_client = echo_client_create(&sock, &handler);
    if (echo_client == NULL) {
        printf("create echo struct error.\n");

        return -1;
    }

    printf("client send message: hello\n");
    echo_client_send_message(echo_client, "hello", 6);
    printf("client send message: world\n");
    echo_client_send_message(echo_client, "test", 5);

    read_size = (int) recv(sock, buffer, 50, 0);
    echo_client_input(echo_client, buffer, read_size);
    read_size = (int) recv(sock, buffer, 50, 0);
    echo_client_input(echo_client, buffer, read_size);

    return 0;
}

int main()
{

    int sock[2];

    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sock) != 0) {
        printf("create socketpair error.\n");

        return -1;
    }

    int pid = fork();
    if (pid == 0) {
        echo_client_test(sock[0]);
    } else {
        echo_server_test(sock[1]);
    }

    sleep(1);

    return 0;
}
