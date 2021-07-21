//
// Created by BoringWednesday on 2021/7/7.
//

#include "amf0.h"

#include <assert.h>
#include <string.h>

static uint8_t *amf0_write_int16(uint8_t *ptr, const uint8_t *end, uint16_t value)
{
    if (ptr + 2 > end) {
        return NULL;
    }

    ptr[0] = value >> 8;
    ptr[1] = value & 0xFF;

    return ptr + 2;
}

static uint8_t *amf0_write_int32(uint8_t *ptr, const uint8_t *end, uint32_t value)
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

static uint8_t *amf0_write_string16(uint8_t *ptr, const uint8_t *end, const char *str, size_t length)
{
    if (ptr + 2 + length > end) {
        return NULL;
    }

    ptr = amf0_write_int16(ptr, end, (uint16_t) length);
    memcpy(ptr, str, length);

    return ptr + length;
}

static uint8_t *amf0_write_string32(uint8_t *ptr, const uint8_t *end, const char *str, size_t length)
{
    if (ptr + 4 + length > end) {
        return NULL;
    }

    ptr = amf0_write_int32(ptr, end, (uint16_t) length);
    memcpy(ptr, str, length);

    return ptr + length;
}

uint8_t *amf0_write_null(uint8_t *ptr, const uint8_t *end)
{
    if (!ptr || ptr + 1 > end) {
        return NULL;
    }

    *ptr++ = AMF0_NULL;

    return ptr;
}

uint8_t *amf0_write_undefined(uint8_t *ptr, const uint8_t *end)
{
    if (!ptr || ptr + 1 > end) {
        return NULL;
    }

    *ptr++ = AMF0_UNDEFINED;

    return ptr;
}

uint8_t *amf0_write_object(uint8_t *ptr, const uint8_t *end)
{
    if (!ptr || ptr + 1 > end) {
        return NULL;
    }

    *ptr++ = AMF0_OBJECT;

    return ptr;
}

uint8_t *amf0_write_object_end(uint8_t* ptr, const uint8_t* end)
{
    if (!ptr || ptr + 1 > end) {
        return NULL;
    }

    // End of Object: 0x00 0x00 0x09
    *ptr++ = 0;
    *ptr++ = 0;
    *ptr++ = AMF0_OBJECT_END;

    return ptr;
}

uint8_t *amf0_write_typed_object(uint8_t* ptr, const uint8_t* end)
{
    if (!ptr || ptr + 1 > end) {
        return NULL;
    }

    *ptr++ = AMF0_TYPED_OBJECT;

    return ptr;
}

uint8_t *amf0_write_ecma_array(uint8_t* ptr, const uint8_t* end)
{
    if (!ptr || ptr + 1 > end) {
        return NULL;
    }

    *ptr++ = AMF0_ECMA_ARRAY;

    // associative-count 0x0000 0000
    return amf0_write_int32(ptr, end, 0);
}

uint8_t *amf0_write_boolean(uint8_t *ptr, const uint8_t *end, uint8_t value)
{
    if (!ptr || ptr + 2 > end) {
        return NULL;
    }

    ptr[0] = AMF0_BOOLEAN;
    ptr[1] = value == 0 ? 0 : 1;

    return ptr + 2;
}

uint8_t *amf0_write_double(uint8_t *ptr, const uint8_t *end, double value)
{
    uint8_t *double_ptr;

    if (!ptr || ptr + 9 > end) {
        return NULL;
    }

    double_ptr = (uint8_t *) &value;

    *ptr++ = AMF0_NUMBER;

    *ptr++ = double_ptr[7];
    *ptr++ = double_ptr[6];
    *ptr++ = double_ptr[5];
    *ptr++ = double_ptr[4];
    *ptr++ = double_ptr[3];
    *ptr++ = double_ptr[2];
    *ptr++ = double_ptr[1];
    *ptr++ = double_ptr[0];

    return ptr;
}

uint8_t *amf0_write_string(uint8_t *ptr, const uint8_t *end, const char *string, size_t length)
{
    if (!ptr || ptr + 1 + (length <= UINT16_MAX ? 2 : 4) + length > end || length > UINT32_MAX) {
        return NULL;
    }

    if (length <= UINT16_MAX) {
        *ptr++ = AMF0_STRING;

        return amf0_write_string16(ptr, end, string, length);
    } else {
        *ptr++ = AMF0_LONG_STRING;

        return amf0_write_string32(ptr, end, string, length);
    }
}

uint8_t *amf0_write_date(uint8_t *ptr, const uint8_t *end, double milliseconds, int16_t timezone)
{

}

uint8_t *amf0_write_named_string(uint8_t *ptr, const uint8_t *end, const char *name, size_t length, const char *value, size_t length2)
{

}

uint8_t *amf0_write_named_double(uint8_t *ptr, const uint8_t *end, const char *name, size_t length, double value)
{

}

uint8_t *amf0_write_named_boolean(uint8_t *ptr, const uint8_t *end, const char *name, size_t length, uint8_t value)
{

}


const uint8_t *amf0_read_null(const uint8_t *ptr, const uint8_t *end)
{

}

const uint8_t *amf0_read_undefined(const uint8_t *ptr, const uint8_t *end)
{

}

const uint8_t *amf0_read_boolean(const uint8_t *ptr, const uint8_t *end, uint8_t *value
{

}

const uint8_t *amf0_read_double(const uint8_t *ptr, const uint8_t *end, double *value
{

}

const uint8_t *amf0_read_string(const uint8_t *ptr, const uint8_t *end, int isLongString, char *string, size_t length
{

}

const uint8_t *amf0_read_date(const uint8_t *ptr, const uint8_t *end, double *milliseconds, int16_t *timezone
{

}
