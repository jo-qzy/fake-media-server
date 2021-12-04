//
// Created by quzhenyu on 2021/12/3.
//

#include "flv_demuxer.h"

#include <stdlib.h>
#include <memory.h>

struct flv_demuxer_t
{
    void                   *param;
    flv_demuxer_handler     handler;
    uint8_t                *buffer;
    uint32_t                capacity;

    // union
    // {
    //     struct mpeg4_avc_t avc;
    // } video;

    // union
    // {
    //     struct mpeg4_aac_t aac;
    // } audio;
};

flv_demuxer_t *flv_demuxer_create(flv_demuxer_handler handler, void *param)
{
    struct flv_demuxer_t *demuxer;

    demuxer = (struct flv_demuxer_t *) malloc(sizeof(struct flv_demuxer_t));
    if (NULL == demuxer)
        return NULL;

    memset(demuxer, 0, sizeof(struct flv_demuxer_t));
    demuxer->handler = handler;
    demuxer->param = param;

    return demuxer;
}

void flv_demuxer_destroy(flv_demuxer_t *demuxer)
{
    if (!demuxer)
        return;

    if (demuxer->buffer)
        free(demuxer->buffer);

    free(demuxer);
}

int flv_demuxer_input(flv_demuxer_t *demuxer, int type, const void *data, size_t bytes, uint32_t timestamp)
{
    switch (type) {
        case (FLV_)
    }
}
