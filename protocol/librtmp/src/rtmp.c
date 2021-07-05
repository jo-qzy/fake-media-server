//
// Created by BoringWednesday on 2021/5/12.
//

#include "rtmp.h"
#include "rtmp_chunk.h"
#include "rtmp_handshake.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
 * 1. handshake (p10)
 * C -> S: C0/C1
 * S -> C: S0/S1/S2
 * C -> S: C2
 *
 * 2. connect (p34)
 * C -> S: connect
 * S -> C: Window Acknowledgement Size
 * S -> C: Set Peer Bandwidth
 * C -> S: Window Acknowledgement Size
 * S -> C: User Control Message (StreamBegin)
 * S -> C: _result(connect response)
 *
 * 3. play (p41)
 * C -> S: createStream
 * S -> C: _result(createStream response)
 * C -> S: play
 * C -> S: SetBufferLength (optional)
 * S -> C: SetChunkSize
 * S -> C: User Control (StreamIsRecorded)
 * S -> C: User Control (StreamBegin)
 * S -> C: onStatus (play reset) (only play set reset flag)
 * S -> C: onStatus (play start)
 * S -> C: Audio
 * S -> C: Video
 *
 * 4. publish (p49)
 * C -> S: createStream
 * S -> C: _result(createStream response)
 * C -> S: publish
 * S -> C: User Control (StreamBegin)
 * C -> S: Metadata
 * C -> S: Audio
 * C -> S: SetChunkSize
 * S -> C: onStatus(publish result)
 * C -> S: Video
 */

struct rtmp_t *rtmp_create(void *param, int is_client, const struct rtmp_handler_t *handler)
{
    struct rtmp_t *ctx;

    ctx = (struct rtmp_t *) calloc(1, sizeof(struct rtmp_t));
    if (!ctx) {
        return NULL;
    }

    ctx->param = param;

    ctx->is_client = is_client;
    ctx->status = RTMP_STATUS_UNINITIALIZED;
    ctx->handshake_status = RTMP_HANDSHAKE_UNINITIALIZED;
    ctx->in_chunk_size = RTMP_CHUNK_SIZE;
    ctx->out_chunk_size = RTMP_CHUNK_SIZE;

    ctx->on_send = handler->on_send;

    return ctx;
}

void rtmp_destroy(struct rtmp_t *rtmp)
{
    if (!rtmp) {
        return;
    }

    free(rtmp);
}

int rtmp_get_status(struct rtmp_t *rtmp)
{
    return rtmp->status;
}

static int rtmp_send_connect(struct rtmp_t *rtmp)
{
    return 0;
}

static int rtmp_process_handshake(struct rtmp_t *rtmp, const void *data, size_t bytes)
{
    size_t          read_bytes, send_bytes;
    int             ret;
    const uint8_t  *ptr;

    ptr = data;

    while (bytes > 0) {
        switch (rtmp->handshake_status) {
            case RTMP_HANDSHAKE_UNINITIALIZED:
                rtmp->handshake_status = RTMP_HANDSHAKE_0;
                rtmp->handshake[0] = *ptr;
                rtmp->handshake_bytes = 1;

                bytes -= 1;
                ptr++;

                break;

            case RTMP_HANDSHAKE_0:
                read_bytes = 1 + RTMP_HANDSHAKE_LENGTH - rtmp->handshake_bytes;
                read_bytes = read_bytes <= bytes ? read_bytes : bytes;

                memmove(rtmp->handshake + rtmp->handshake_bytes, ptr, bytes);
                rtmp->handshake_bytes += read_bytes;
                bytes -= read_bytes;
                ptr += read_bytes;

                if (rtmp->handshake_bytes == 1 + RTMP_HANDSHAKE_LENGTH) {
                    rtmp->handshake_status = RTMP_HANDSHAKE_1;

                    if (rtmp->is_client) {
                        // Client receive S0 + S1, need return C2
                        ret = rtmp_send_handshake_c2(rtmp);
                        if (ret != 0) {
                            return ret;
                        }

                    } else {
                        // Server receive C0 + C1, need return S0 + S1 + s2
                        ret = rtmp_send_handshake_s0_s1_s2(rtmp);
                        if (ret != 0) {
                            return ret;
                        }

                    }
                }

                break;

            case RTMP_HANDSHAKE_1:
                read_bytes = 1 + RTMP_HANDSHAKE_LENGTH * 2 - rtmp->handshake_bytes;
                read_bytes = read_bytes <= bytes ? read_bytes : bytes;

                memmove(rtmp->handshake + rtmp->handshake_bytes, ptr, bytes);
                rtmp->handshake_bytes += read_bytes;
                bytes -= read_bytes;
                ptr += read_bytes;

                if (rtmp->handshake_bytes == 1 + RTMP_HANDSHAKE_LENGTH * 2) {
                    rtmp->handshake_status = RTMP_HANDSHAKE_2;
                    rtmp->status = RTMP_STATUS_HANDSHAKE;
                    rtmp->handshake_bytes = 0;

                    if (rtmp->is_client) {
                        ret = rtmp_send_connect(rtmp);
                        if (ret != 0) {
                            return ret;
                        }

                    }
                }

                break;

            case RTMP_HANDSHAKE_2:
            default:
                // After finish handshake still remain some bytes
                return rtmp_chunk_read(rtmp, data, bytes);
        }
    }

    // Need more data
    return 0;
}

int rtmp_input(struct rtmp_t *rtmp, const void *data, size_t bytes)
{
    if (data == NULL || bytes == 0) {
        return -1;
    }

    if (rtmp->status == RTMP_STATUS_UNINITIALIZED) {
        return rtmp_process_handshake(rtmp, data, bytes);
    }

    return rtmp_chunk_read(rtmp, data, bytes);
}

int rtmp_connect(struct rtmp_t *rtmp)
{
    int send_bytes;

    if (rtmp->status > RTMP_STATUS_HANDSHAKE) {
        return -1;
    }

    if (!rtmp->is_client) {
        return 0;
    }

    return rtmp_send_handshake_c0_c1(rtmp);
}
