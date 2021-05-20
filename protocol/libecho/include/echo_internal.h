//
// Created by BoringWednesday on 2021/5/12.
//

#ifndef FAKE_MEDIA_SERVER_ECHO_CLIENT_H
#define FAKE_MEDIA_SERVER_ECHO_CLIENT_H


#include <stddef.h>
#include <stdint.h>

struct echo_packet_t
{
    uint8_t     buffer[1024];
    uint32_t    bytes;
};

struct echo_t
{
    uint32_t                sequence;
    void                   *param;
    struct echo_packet_t    in_packet;
};


#endif //FAKE_MEDIA_SERVER_ECHO_CLIENT_H
