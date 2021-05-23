//
// Created by BoringWednesday on 2021/5/23.
//

#include "echo_test.h"
#include <unistd.h>
#include <echo_client.h>
#include <arpa/inet.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    struct echo_client_t           *echo_client;
    struct echo_client_handler_t    handler;
    char        buffer[50];
    int         read_size;
    int         sock;
    struct sockaddr_in address;

    if (argc != 3) {
        printf("Usage:\n");
        printf("\t./echo_test_client ip port\n");

        return -1;
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printf("Create socket failed.\n");

        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_port = htons(atoi(argv[2]));
    address.sin_addr.s_addr = inet_addr(argv[1]);

    if (connect(sock, (struct sockaddr *) &address, sizeof address) != 0) {
        return -1;
    }

    handler.send = echo_send;
    handler.onpacket = echo_onpacket;

    echo_client = echo_client_create(&sock, &handler);
    if (echo_client == NULL) {
        printf("create echo struct error.\n");

        return -1;
    }

    printf("client send message: hello\n");
    echo_client_send_message(echo_client, "hello", 6);
    printf("client send message: world!\n");
    echo_client_send_message(echo_client, "world!", 7);

    sleep(1);

    read_size = (int) recv(sock, buffer, 50, 0);
    echo_client_input(echo_client, buffer, read_size);

    close(sock);

    return 0;
}
