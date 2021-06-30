//
// Created by BoringWednesday on 2021/6/29.
//

#ifndef FAKE_MEDIA_SERVER_RTMP_TEST_H
#define FAKE_MEDIA_SERVER_RTMP_TEST_H

#include <stdio.h>
#include <sys/socket.h>
#include <stdint.h>

int rtmp_send(void *param, const uint8_t *header, uint32_t header_bytes, const uint8_t *payload, uint32_t payload_bytes)
{
    int sock = *(int *) param;
    struct msghdr msg_handler;
    struct iovec message[2];

    msg_handler.msg_name = NULL;
    msg_handler.msg_namelen = 0;
    msg_handler.msg_iov = message;
    msg_handler.msg_iovlen = 2;
    message[0].iov_base = (void *) header;
    message[0].iov_len = header_bytes;
    message[1].iov_base = (void *) payload;
    message[1].iov_len = payload_bytes;


    return (int) sendmsg(sock, &msg_handler, 0);
}

//int rtmp_onpacket(void *param, const void *data, size_t bytes, uint32_t sequence)
//{
//    printf("Receive sequence: %d message: %s bytes: %zu\n", sequence, (const char *) data, bytes);
//
//    return 0;
//}

#endif //FAKE_MEDIA_SERVER_RTMP_TEST_H
