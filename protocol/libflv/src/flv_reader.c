//
// Created by quzhenyu on 2021/12/3.
//

#include "flv_reader.h"
#include "flv_header.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

struct flv_reader_t
{
    void               *param;
    FILE               *file;
    flv_reader_handler  read;
};

static int flv_reader_read_file(void *param, void *buf, uint32_t len)
{
    return (int) fread(buf, 1, len, param);
}

static int flv_reader_read_header(flv_reader_t *reader)
{
    uint8_t buffer[FLV_HEADER_SIZE];
    uint32_t previous_size;
    flv_header_t header;

    if (FLV_HEADER_SIZE != reader->read(reader->param, buffer, FLV_HEADER_SIZE))
        return -1;

    // Parse flv header failed, not flv file
    if (FLV_HEADER_SIZE != flv_header_read(&header, buffer, FLV_HEADER_SIZE))
        return -1;

    // Skip header data offset
    if (header.data_offset > FLV_HEADER_SIZE) {
        if (0 != fseek(reader->param, (long) header.data_offset, SEEK_SET))
            return -1;
    }

    // Skip first previous tag size
    if (FLV_PREVIOUS_TAG_LENGTH != reader->read(reader->param, &previous_size, FLV_PREVIOUS_TAG_LENGTH))
        return -1;

    if (0 != previous_size)
        return -1;

    return 0;
}

flv_reader_t *flv_reader_create(const char *file, flv_reader_handler handler, void *param)
{
    struct flv_reader_t *reader;

    reader = (struct flv_reader_t *) malloc(sizeof(struct flv_reader_t));
    if (NULL == reader)
        return NULL;

    memset(reader, 0, sizeof(struct flv_reader_t));

    reader->param = param;

    if (handler)
        reader->read = handler;
    else {
        FILE *flv_file = fopen(file, "rb");
        if (!flv_file) {
            flv_reader_destroy(reader);

            return NULL;
        }

        reader->param = flv_file;
        reader->file = flv_file;
        reader->read = flv_reader_read_file;
    }

    if (flv_reader_read_header(reader) != 0) {
        flv_reader_destroy(reader);

        return NULL;
    }

    return reader;
}

void flv_reader_destroy(flv_reader_t *reader)
{
    if (reader->file)
        fclose(reader->file);

    free(reader);
}

int flv_reader_read(flv_reader_t *reader, int *tag_type, uint32_t *timestamp, uint32_t *taglen,
                    void *buffer, uint32_t bytes)
{
    flv_tag_t tag;
    uint8_t tag_header[FLV_TAG_HEADER_SIZE];
    uint32_t previous_size;
    int read_size;

    read_size = reader->read(reader->param, tag_header, FLV_TAG_HEADER_SIZE);
    if (FLV_TAG_HEADER_SIZE != read_size)
        return read_size < 0 ? read_size : 0;

    if (FLV_TAG_HEADER_SIZE != flv_tag_read(&tag, tag_header, FLV_TAG_HEADER_SIZE))
        return -1;

    if (bytes < tag.data_size)
        return -1;

    read_size = reader->read(reader->param, buffer, tag.data_size);
    if (tag.data_size != read_size)
        return read_size < 0 ? read_size : 0;

    read_size = reader->read(reader->param, &previous_size, FLV_PREVIOUS_TAG_LENGTH);
    if (FLV_PREVIOUS_TAG_LENGTH != read_size)
        return read_size < 0 ? read_size : 0;

    *taglen = tag.data_size;
    *tag_type = tag.tag_type;
    *timestamp = tag.timestamp;

    if (FLV_PREVIOUS_TAG_LENGTH !=
        flv_tag_size_read((uint8_t *) &previous_size, FLV_PREVIOUS_TAG_LENGTH, &previous_size))
        return -1;

    return (previous_size == (tag.data_size + FLV_TAG_HEADER_SIZE)) ? 1 : -1;
}
