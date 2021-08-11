//
// Created by BoringWednesday on 2021/8/9.
//

#ifndef LIBFLV_FLV_WIRTER_H
#define LIBFLV_FLV_WIRTER_H

#include <stddef.h>
#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

#include "flv.h"

typedef struct flv_writer_handler_t
{
    /// @param[in] param User parameters
    /// @param[in] header flv tag header
    /// @param[in] header_len flv tag header length
    /// @param[in] data flv audio/video data
    /// @param[in] bytes length of audio/video data
    /// @return 0-ok, other-error
    int (*on_write)(void *param, flv_vec_t *vec, size_t len);
} flv_writer_handler_t;

typedef struct flv_writer_t
{
    void   *param;

    int (*on_write)(void *param, flv_vec_t *vec, size_t len);
} flv_writer_t;

flv_writer_t *flv_writer_create(void* param, int audio, int video, flv_writer_handler_t *handler);

void flv_writer_destroy(flv_writer_t *flv);

int flv_writer_input(flv_writer_t *flv, int type, uint32_t timestamp, const void *data, size_t bytes);

#if defined(__cplusplus)
}
#endif

#endif //LIBFLV_FLV_WIRTER_H
