//
// Created by BoringWednesday on 2021/6/29.
//

#include "rtmp_test.h"
#include <rtmp.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>


int main(int argc, char *argv[])
{
    struct rtmp_t *rtmp;
    struct rtmp_handler_t handler;
    struct sockaddr_in address;
    int sock;

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

    handler.on_send = rtmp_send;

    rtmp = rtmp_create(&sock, 1, &handler);
    if (rtmp == NULL) {
        return -1;
    }

    rtmp_connect(rtmp);

    return 0;
}
