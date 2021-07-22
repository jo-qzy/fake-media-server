//
// Created by BoringWednesday on 2021/7/22.
//

#include "amf_util.h"

#include <string.h>

inline uint8_t *amf0_write_int16(uint8_t *ptr, const uint8_t *end, uint16_t value)
{
    if (ptr + 2 > end) {
        return NULL;
    }

    ptr[0] = value >> 8;
    ptr[1] = value & 0xFF;

    return ptr + 2;
}

inline uint8_t *amf0_write_int32(uint8_t *ptr, const uint8_t *end, uint32_t value)
{
    if (ptr + 4 > end) {
        return NULL;
    }

    ptr[0] = (uint8_t) (value >> 24);
    ptr[1] = (uint8_t) (value >> 16);
    ptr[2] = (uint8_t) (value >> 8);
    ptr[3] = (uint8_t) (value & 0xFF);

    return ptr + 4;
}

inline uint8_t *amf0_write_string16(uint8_t *ptr, const uint8_t *end, const char *str, size_t length)
{
    if (ptr + 2 + length > end || !str) {
        return NULL;
    }

    ptr = amf0_write_int16(ptr, end, (uint16_t) length);
    memcpy(ptr, str, length);

    return ptr + length;
}

inline uint8_t *amf0_write_string32(uint8_t *ptr, const uint8_t *end, const char *str, size_t length)
{
    if (ptr + 4 + length > end || !str) {
        return NULL;
    }

    ptr = amf0_write_int32(ptr, end, (uint16_t) length);
    memcpy(ptr, str, length);

    return ptr + length;
}
