//
// Created by BoringWednesday on 2021/5/12.
//

#include "echo_internal.h"
#include "echo_client.h"
#include <memory.h>
#include <stdlib.h>
#include <stdint.h>

/* |    4 bytes    |    4 bytes    |
 * |-------------------------------|
 * | sequence num  |     length    |
 * |          payload....          |
 * |          ...........          |
 * */

struct echo_client_t
{
    struct echo_t                   echo;
    uint8_t                         payload[1024];
    void                           *param;
    struct echo_client_handler_t    handler;
};

struct echo_client_t * echo_client_create(void *param, struct echo_client_handler_t *handler)
{
    struct echo_client_t *ctx;

    ctx = (struct echo_client_t *) calloc(1, sizeof(*ctx));
    if (ctx == NULL || handler == NULL) {
        return NULL;
    }

    ctx->echo.sequence = 0;
    ctx->echo.param = ctx;
    ctx->echo.in_packet.bytes = 0;
    ctx->param = param;
    memcpy(&ctx->handler, handler, sizeof(ctx->handler));

    return ctx;
}

int echo_destroy(struct echo_client_t *ctx)
{
    if (ctx) {
        free(ctx);
    }

    return 0;
}

int echo_client_input(struct echo_client_t *ctx, const void *data, size_t bytes)
{
    /* Skip sequence parse */
    uint32_t                length;
    size_t                  bytes_read, read_size;
    struct echo_t          *echo;
    struct echo_packet_t   *pkt;

    bytes_read = read_size = 0;
    pkt = &ctx->echo.in_packet;
    echo = &ctx->echo;

    while (bytes_read < bytes) {
        if (pkt->bytes <= 8) {
            read_size = 8 - pkt->bytes <= bytes - bytes_read ? 8 - pkt->bytes : bytes;

            memcpy(pkt->buffer + pkt->bytes, data + bytes_read, read_size);

            bytes_read += read_size;
            pkt->bytes += read_size;

            if (pkt->bytes < 8) {
                return 0;
            }
        }

        length = ((uint32_t) pkt->buffer[4] << 24) |
                 ((uint32_t) pkt->buffer[5] << 16) |
                 ((uint32_t) pkt->buffer[6] << 8) |
                 ((uint32_t) pkt->buffer[7]);

        read_size = length - (pkt->bytes - 8);
        if (read_size <= bytes - bytes_read) {
            memcpy(pkt->buffer + pkt->bytes, data + bytes_read, read_size);
            ctx->handler.onpacket(ctx->param, pkt->buffer + 8, length, echo->sequence);

            echo->sequence++;
            pkt->bytes = 0;
            bytes_read += read_size;

        } else {
            read_size = bytes - bytes_read;
            memcpy(pkt->buffer + pkt->bytes, pkt->buffer + 8, read_size);

            pkt->bytes += read_size;

            return 0;
        }
    }

    return (int) bytes;
}

int echo_client_send_message(struct echo_client_t *ctx, const void* data, size_t bytes)
{
    struct echo_t  *echo;
    uint8_t         vec[8];

    echo = &ctx->echo;

    vec[0] = (echo->sequence >> 24) & 0xFF;
    vec[1] = (echo->sequence >> 16) & 0xFF;
    vec[2] = (echo->sequence >> 8) & 0xFF;
    vec[3] = echo->sequence & 0xFF;
    vec[4] = (bytes >> 24) & 0xFF;
    vec[5] = (bytes >> 16) & 0xFF;
    vec[6] = (bytes >> 8) & 0xFF;
    vec[7] = bytes & 0xFF;

    memcpy(ctx->payload, vec, 8);
    memcpy(ctx->payload + 8, data, bytes);

    return ctx->handler.send(ctx->param, ctx->payload, bytes + 8);
}
