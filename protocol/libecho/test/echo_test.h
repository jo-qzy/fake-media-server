//
// Created by BoringWednesday on 2021/5/23.
//

#ifndef FAKE_MEDIA_SERVER_ECHO_TEST_H
#define FAKE_MEDIA_SERVER_ECHO_TEST_H

#include <stdio.h>
#include <sys/socket.h>
#include <stdint.h>

int echo_send(void *param, const void *packet, size_t bytes)
{
    int sock = *(int *) param;

    return (int) send(sock, packet, bytes, 0);
}

int echo_onpacket(void *param, const void *data, size_t bytes, uint32_t sequence)
{
    printf("Receive sequence: %d message: %s bytes: %zu\n", sequence, (const char *) data, bytes);

    return 0;
}

#endif //FAKE_MEDIA_SERVER_ECHO_TEST_H
