//
// Created by BoringWednesday on 2021/6/22.
//

#ifndef LIBRTMP_CHUNK_H
#define LIBRTMP_CHUNK_H


#include "rtmp.h"


int rtmp_chunk_read(struct rtmp_t *rtmp, const uint8_t *data, size_t bytes);


#endif //LIBRTMP_CHUNK_H
