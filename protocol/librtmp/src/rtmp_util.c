//
// Created by BoringWednesday on 2021/6/17.
//

#include "rtmp_util.h"


inline void rtmp_write_uint16_be(uint8_t *ptr, uint16_t val)
{
    ptr[0] = (uint8_t) ((val >> 8) &0xFF);
    ptr[1] = (uint8_t) (val & 0xFF);
}

inline void rtmp_write_uint24_be(uint8_t *ptr, uint32_t val)
{
    ptr[0] = (uint8_t) ((val >> 16) &0xFF);
    ptr[1] = (uint8_t) ((val >> 8) &0xFF);
    ptr[2] = (uint8_t) (val & 0xFF);
}

inline void rtmp_write_uint32_be(uint8_t *ptr, uint32_t val)
{
    ptr[0] = (uint8_t) ((val >> 24) &0xFF);
    ptr[1] = (uint8_t) ((val >> 16) &0xFF);
    ptr[2] = (uint8_t) ((val >> 8) &0xFF);
    ptr[3] = (uint8_t) (val & 0xFF);
}

inline void rtmp_read_uint16_be(uint8_t *ptr, uint16_t *val)
{
    *val = (ptr[0] << 8) | ptr[1];
}

inline void rtmp_read_uint24_be(uint8_t *ptr, uint32_t *val)
{
    *val = (ptr[0] << 16) | (ptr[1] << 8) | ptr[2];
}

inline void rtmp_read_uint32_be(uint8_t *ptr, uint32_t *val)
{
    *val = (ptr[0] << 24) | (ptr[1] << 16) | (ptr[2] << 8) | ptr[3];
}

inline void rtmp_write_uint32_le(uint8_t *ptr, uint32_t val)
{
    ptr[0] = (uint8_t) (val & 0xFF);
    ptr[1] = (uint8_t) ((val >> 8) &0xFF);
    ptr[2] = (uint8_t) ((val >> 16) &0xFF);
    ptr[3] = (uint8_t) ((val >> 24) &0xFF);
}

inline void rtmp_read_uint32_le(uint32_t *ptr, uint32_t *val)
{
    *val = ptr[0] | (ptr[1] << 8) | (ptr[2] << 16) | (ptr[3] << 24);
}
