//
// Created by BoringWednesday on 2021/6/29.
//

#include "rtmp_test.h"


int main(int argc, char *argv[])
{
    struct rtmp_t *rtmp;
    struct rtmp_handler_t handler;
    struct sockaddr_in address;
    int sock, recv_bytes, ret;
    uint8_t buffer[2 * 1024 * 1024];

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

    while (1) {
        recv_bytes = (int) recv(sock, buffer, 2 * 1024 * 1024, 0);
        if (recv_bytes <= 0) {
            if (recv_bytes == 0) {
                printf("connection broken\n");

                break;
            } else if (errno != EAGAIN) {
                printf("recv occur error, errno: %d\n", errno);

                break;
            }
        }

        printf("recv bytes: %d\n", recv_bytes);

        ret = rtmp_input(rtmp, buffer, recv_bytes);
        if (ret < 0) {
            printf("rtmp_input return wrong value: %d\n", recv_bytes);

            break;
        }

        if (rtmp_get_status(rtmp) == RTMP_STATUS_HANDSHAKE) {
            printf("handshake success\n");

            break;
        }
    }

    rtmp_destroy(rtmp);

    return 0;
}
