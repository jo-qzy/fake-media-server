//
// Created by BoringWednesday on 2021/6/17.
//

#ifndef LIBRTMP_HANDSHAKE_H
#define LIBRTMP_HANDSHAKE_H


#include <stddef.h>
#include <stdint.h>


#define RTMP_VERSION 3

#define RTMP_HANDSHAKE_LENGTH 1536

struct rtmp_t;

typedef enum rtmp_handshake_status_t
{
    RTMP_HANDSHAKE_UNINITIALIZED    = 0, // Handshake uninitialized
    RTMP_HANDSHAKE_0                = 1, // Received C0/S0, wait C1/S1
    RTMP_HANDSHAKE_1                = 2, // Received C1/S1, wait C2/S2
    RTMP_HANDSHAKE_2                = 3  // Received C2/S2
} rtmp_handshake_status_t;


int rtmp_handshake_c0(uint8_t *c0, int version);
int rtmp_handshake_c1(uint8_t *c1, uint32_t timestamp);
int rtmp_handshake_c2(uint8_t *c2, uint32_t timestamp, const uint8_t *s1, size_t s1_bytes);

int rtmp_handshake_s0(uint8_t *s0, int version);
int rtmp_handshake_s1(uint8_t *s1, uint32_t timestamp);
int rtmp_handshake_s2(uint8_t *s2, uint32_t timestamp, const uint8_t *c1, size_t c1_bytes);

int rtmp_send_handshake_c0_c1(struct rtmp_t *rtmp);
int rtmp_send_handshake_c2(struct rtmp_t *rtmp);

int rtmp_send_handshake_s0_s1_s2(struct rtmp_t *rtmp);


#endif //LIBRTMP_HANDSHAKE_H
