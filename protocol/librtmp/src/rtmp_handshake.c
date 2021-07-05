//
// Created by BoringWednesday on 2021/6/7.
//

#include "rtmp.h"
#include "rtmp_handshake.h"
#include "rtmp_util.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
 * ------------- RTMP Handshake -------------
 * --------------- Client side --------------
 * Send C0 + C1
 *  C0: 0x03(RTMP) 0x08(RTMPE)
 *  C1: timestamp       4 bytes
 *      zero            4 bytes
 *      random bytes    1528 bytes
 * Receive S0 + S1
 *  S0: 0x03(RTMP) 0x08(RTMPE)
 *  S1: timestamp       4 bytes
 *      zero            4 bytes
 *      random bytes    1528 bytes
 *
 * Send C2
 *  C2: C2 is a copy of S1    1536 bytes
 *
 * Receive S2
 *  C2: C2 is a copy of S1    1536 bytes
 *
 * --------- RTMP Complex Handshake ---------
 * --------------- Client side --------------
 * TODO: complex handshake
 * http://blog.csdn.net/win_lin/article/details/13006803
 */


static void rtmp_handshake_random(uint8_t *ptr, uint32_t timestamp)
{
    int i;

    srand(timestamp);

    /* Generate random 1528 bytes */
    for (i = 0; i * 4 < RTMP_HANDSHAKE_LENGTH - 8; i++) {
        *((int *) ptr + i) = rand();
    }
}

int rtmp_handshake_c0(uint8_t *c0, int version)
{
    if (c0 == NULL) {
        return -1;
    }

    assert(RTMP_VERSION == version);

    /* version: 1 byte (0x03 RTMP) */
    *c0 = (uint8_t) version;

    return 1;
}

int rtmp_handshake_c1(uint8_t *c1, uint32_t timestamp)
{
    if (c1 == NULL) {
        return -1;
    }

    /* timestamp: 4 bytes */
    rtmp_write_uint32_be(c1, timestamp);

    /* zero: 4 bytes */
    memset(c1 + 4, 0, 4);

    /* random bytes: 1528 bytes */
    rtmp_handshake_random(c1 + 8, timestamp);

    return RTMP_HANDSHAKE_LENGTH;
}

int rtmp_handshake_c2(uint8_t *c2, uint32_t timestamp, const uint8_t *s1, size_t s1_bytes)
{
    if (c2 == NULL || s1_bytes != RTMP_HANDSHAKE_LENGTH) {
        return -1;
    }

    memmove(c2, s1, s1_bytes);

    rtmp_write_uint32_be(c2 + 4, timestamp);

    return RTMP_HANDSHAKE_LENGTH;
}

int rtmp_handshake_s0(uint8_t *s0, int version)
{
    if (s0 == NULL) {
        return -1;
    }

    assert(RTMP_VERSION == version);

    *s0 = (uint8_t) version;

    return 1;
}

int rtmp_handshake_s1(uint8_t *s1, uint32_t timestamp)
{
    if (s1 == NULL) {
        return -1;
    }

    /* timestamp: 4 bytes */
    rtmp_write_uint32_be(s1, timestamp);

    /* zero: 4 bytes */
    memset(s1 + 4, 0, 4);

    /* random bytes: 1528 bytes */
    rtmp_handshake_random(s1 + 8, timestamp);

    return RTMP_HANDSHAKE_LENGTH;
}

int rtmp_handshake_s2(uint8_t *s2, uint32_t timestamp, const uint8_t *c1, size_t c1_bytes)
{
    if (s2 == NULL || c1_bytes != RTMP_HANDSHAKE_LENGTH) {
        return -1;
    }

    memmove(s2, c1, c1_bytes);

    rtmp_write_uint32_be(s2 + 4, timestamp);

    return RTMP_HANDSHAKE_LENGTH;
}

int rtmp_send_handshake_c0_c1(struct rtmp_t *rtmp)
{
    size_t send_bytes;

    if (!rtmp) {
        return -1;
    }

    send_bytes = rtmp_handshake_c0(rtmp->payload, RTMP_VERSION);
    send_bytes += rtmp_handshake_c1(rtmp->payload + 1, (uint32_t) time(NULL));

    return send_bytes == rtmp->on_send(rtmp->param, rtmp->payload, send_bytes, NULL, 0) ? 0 : -1;
}

int rtmp_send_handshake_c2(struct rtmp_t *rtmp)
{
    size_t send_bytes;

    if (!rtmp || rtmp->handshake_bytes != 1 + RTMP_HANDSHAKE_LENGTH) {
        return -1;
    }

    send_bytes = rtmp_handshake_c2(rtmp->payload, (uint32_t) time(NULL), rtmp->payload, RTMP_HANDSHAKE_LENGTH);

    return send_bytes == rtmp->on_send(rtmp->param, rtmp->payload, send_bytes, NULL, 0) ? 0 : -1;
}

int rtmp_send_handshake_s0_s1_s2(struct rtmp_t *rtmp)
{
    size_t send_bytes;

    if (!rtmp || rtmp->payload_bytes != RTMP_HANDSHAKE_LENGTH) {
        return -1;
    }

    send_bytes = rtmp_handshake_s0(rtmp->payload, RTMP_VERSION);
    send_bytes += rtmp_handshake_s1(rtmp->payload + 1, (uint32_t) time(NULL));
    send_bytes += rtmp_handshake_s2(rtmp->payload + send_bytes, (uint32_t) time(NULL),
                                    rtmp->handshake, RTMP_HANDSHAKE_LENGTH);

    return send_bytes == rtmp->on_send(rtmp->param, rtmp->payload, send_bytes, NULL, 0) ? 0 : -1;
}
