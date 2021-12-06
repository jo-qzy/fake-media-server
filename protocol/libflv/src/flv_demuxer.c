//
// Created by quzhenyu on 2021/12/3.
//

#include "flv_demuxer.h"
#include "flv_header.h"
#include "flv_type.h"

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

static int flv_demuxer_audio(flv_demuxer_t *demuxer, const void *data, size_t bytes, uint32_t timestamp)
{
    flv_audio_tag_header_t audio_header;
    int read_size;

    read_size = flv_audio_tag_header_read(&audio_header, data, bytes);
    if (-1 == read_size)
        return -1;

    return 0;
}

static int flv_demuxer_video(flv_demuxer_t *demuxer, const void *data, size_t bytes, uint32_t timestamp)
{
    flv_video_tag_header_t video_header;
    int read_size;

    read_size = flv_video_tag_header_read(&video_header, data, bytes);
    if (-1 == read_size)
        return -1;

    switch (video_header.codec_id) {
        case FLV_VIDEO_H264:
            if (FLV_SEQUENCE_HEADER == video_header.avc_packet_type) {
                // ISO/IEC 14496-15: AVCDecoderConfigurationRecord
                // ISO/IEC 14496-10: Sequence parameter set RBSP syntax
                // ISO/IEC 14496-10: Picture parameter set RBSP syntax

            }
    }

    return 0;
}

int flv_demuxer_input(flv_demuxer_t *demuxer, int tag_type, const void *data, size_t bytes, uint32_t timestamp)
{
    if (!demuxer)
        return -1;

    switch (tag_type) {
        case FLV_AUDIO:
            return flv_demuxer_audio(demuxer, data, bytes, timestamp);

        case FLV_VIDEO:
            return flv_demuxer_video(demuxer, data, bytes, timestamp);

        case FLV_SCRIPT:
//            return flv_demuxer_script();
        default:
            return -1;
    }
}
