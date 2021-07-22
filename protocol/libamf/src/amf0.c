//
// Created by BoringWednesday on 2021/7/7.
//

#include <assert.h>
#include <string.h>

#include "amf0.h"
#include "amf_util.h"

/*
 * AMF Format:
 *      AMF_TYPE: 1 byte
 *      DATA_LENGTH: 0/2/4 bytes
 *      DATA: N bytes
 */

uint8_t *amf0_write_number(uint8_t *ptr, const uint8_t *end, double value)
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

uint8_t *amf0_write_boolean(uint8_t *ptr, const uint8_t *end, uint8_t value)
{
    if (!ptr || ptr + 2 > end) {
        return NULL;
    }

    *ptr++ = AMF0_BOOLEAN;
    *ptr++ = value == 0 ? 0 : 1;

    return ptr;
}

uint8_t *amf0_write_string(uint8_t *ptr, const uint8_t *end, const char *string, size_t length)
{
    if (!ptr || length > UINT16_MAX || ptr + 1 + 2 + length > end) {
        return NULL;
    }

    *ptr++ = AMF0_STRING;

    return amf0_write_string16(ptr, end, string, length);
}

uint8_t *amf0_write_object(uint8_t *ptr, const uint8_t *end)
{
    if (!ptr || ptr + 1 > end) {
        return NULL;
    }

    *ptr++ = AMF0_OBJECT;

    return ptr;
}

uint8_t *amf0_write_movieclip(uint8_t *ptr, const uint8_t *end)
{
    // Nonsupport AMF type

    return ptr;
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

uint8_t *amf0_write_reference(uint8_t *ptr, const uint8_t *end, uint16_t reference)
{
    if (!ptr || ptr + 3 > end) {
        return NULL;
    }

    *ptr++ = AMF0_REFERENCE;

    return amf0_write_int16(ptr, end, reference);
}

uint8_t *amf0_write_ecma_array(uint8_t* ptr, const uint8_t* end, uint32_t array_length)
{
    if (!ptr || ptr + 1 > end) {
        return NULL;
    }

    *ptr++ = AMF0_ECMA_ARRAY;

    // associative-count: 4 bytes
    return amf0_write_int32(ptr, end, array_length);
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

uint8_t *amf0_write_long_string(uint8_t *ptr, const uint8_t *end, const char *string, size_t length)
{
    if (!ptr || length > UINT32_MAX || ptr + 1 + 4 + length > end) {
        return NULL;
    }

    *ptr++ = AMF0_LONG_STRING;

    return amf0_write_string32(ptr, end, string, length);
}





uint8_t *amf0_write_typed_object(uint8_t* ptr, const uint8_t* end)
{
    if (!ptr || ptr + 1 > end) {
        return NULL;
    }

    *ptr++ = AMF0_TYPED_OBJECT;

    return ptr;
}









uint8_t *amf0_write_date(uint8_t *ptr, const uint8_t *end, double milliseconds, int16_t timezone)
{
    if (!ptr || ptr + 11 > end) {
        return NULL;
    }
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
