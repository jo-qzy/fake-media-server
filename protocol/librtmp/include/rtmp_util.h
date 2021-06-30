//
// Created by BoringWednesday on 2021/6/17.
//

#ifndef LIBRTMP_RTMP_UTIL_H
#define LIBRTMP_RTMP_UTIL_H


#include <stdint.h>


inline void rtmp_write_uint16_be(uint8_t *ptr, uint16_t val);

inline void rtmp_write_uint24_be(uint8_t *ptr, uint32_t val);

inline void rtmp_write_uint32_be(uint8_t *ptr, uint32_t val);

inline void rtmp_read_uint16_be(uint8_t *ptr, uint16_t *val);

inline void rtmp_read_uint24_be(uint8_t *ptr, uint32_t *val);

inline void rtmp_read_uint32_be(uint8_t *ptr, uint32_t *val);

inline void rtmp_write_uint32_le(uint8_t *ptr, uint32_t val);

inline void rtmp_read_uint32_le(uint32_t *ptr, uint32_t *val);


#endif //LIBRTMP_RTMP_UTIL_H