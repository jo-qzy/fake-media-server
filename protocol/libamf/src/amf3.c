//
// Created by BoringWednesday on 2021/7/7.
//

#include <string.h>

#include "amf3.h"

static inline uint8_t *amf3_write_int29(uint8_t *ptr, const uint8_t *end, uint32_t value)
{
    if (0x00000000 <= value && value <= 0x0000007F) {
        if (ptr + 1 > end) {
            return NULL;
        }

        *ptr++ = (uint8_t) value;

        return ptr;

    } else if (0x0000007F < value && value <= 0x00003FFF) {
        if (ptr + 2 > end) {
            return NULL;
        }

        ptr[0] = (uint8_t) (0x80 | (value >> 7));
        ptr[1] = (uint8_t) (0x7F & value);

        return ptr + 2;

    } else if (0x00003FFF < value && value <= 0x001FFFFF) {
        if (ptr + 3 > end) {
            return NULL;
        }

        ptr[0] = (uint8_t) (0x80 | (value >> 14));
        ptr[1] = (uint8_t) (0x80 | (value >> 7));
        ptr[2] = (uint8_t) (0x7F & value);

        return ptr + 3;

    } else if (0x001FFFFF < value && value <= 0x3FFFFFFF) {
        if (ptr + 4 > end) {
            return NULL;
        }

        ptr[0] = (uint8_t) (0x80 | (value >> 22));
        ptr[1] = (uint8_t) (0x80 | (value >> 15));
        ptr[1] = (uint8_t) (0x80 | (value >> 8));
        ptr[3] = (uint8_t) (0xFF & value);

        return ptr + 4;

    }

    return NULL;
}

static inline uint8_t *amf3_write_string29(uint8_t *ptr, const uint8_t *end, const char *str, size_t length)
{
    ptr = amf3_write_int29(ptr, end, (length << 1) | 1);
    if (ptr + length > end) {
        return NULL;
    }

    memcpy(ptr, str, length);

    return ptr + length;
}

uint8_t *amf3_write(uint8_t *ptr, const uint8_t *end, struct amf_object_item_t *item)
{
    if (!ptr || !item) {
        return NULL;
    }

    switch (item->type) {
        case AMF3_UNDEFINED:
            return amf3_write_undefined(ptr, end);
        case AMF3_NULL:
            return amf3_write_null(ptr, end);
        case AMF3_FALSE:
            return amf3_write_false(ptr, end);
        case AMF3_TRUE:
            return amf3_write_true(ptr, end);
        case AMF3_INTEGER:
            return amf3_write_integer(ptr, end, *(uint32_t *) item->value);
        case AMF3_DOUBLE:
            return amf3_write_double(ptr, end, *(double *) item->value);
        case AMF3_STRING:
            AMF_CHECK_POINTER(item->value);

            return amf3_write_string(ptr, end, item->value, strlen(item->value));
        case AMF3_XML_DOCUMENT:
            AMF_CHECK_POINTER(item->value);

            return amf3_write_xml_document(ptr, end, item->value, strlen(item->value));
        case AMF3_DATE:
            return amf3_write_date(ptr, end, *(double *) item->value);
        case AMF3_ARRAY:
            return amf3_write_array(ptr, end, item);
        case AMF3_OBJECT:
            return amf3_write_object(ptr, end, item);
        case AMF3_XML:
            AMF_CHECK_POINTER(item->value);

            return amf3_write_xml(ptr, end, item->value, strlen(item->value));
        case AMF3_BYTEARRAY:
            AMF_CHECK_POINTER(item->value);

            return amf3_write_byte_array(ptr, end, item->value, strlen(item->value));
        default:
            return NULL;
    }
}

uint8_t *amf3_write_undefined(uint8_t *ptr, const uint8_t *end)
{
    if (!ptr || ptr + 1 > end) {
        return NULL;
    }

    *ptr++ = AMF3_UNDEFINED;

    return ptr;
}

uint8_t *amf3_write_null(uint8_t *ptr, const uint8_t *end)
{
    if (!ptr || ptr + 1 > end) {
        return NULL;
    }

    *ptr++ = AMF3_NULL;

    return ptr;
}

uint8_t *amf3_write_false(uint8_t *ptr, const uint8_t *end)
{
    if (!ptr || ptr + 1 > end) {
        return NULL;
    }

    *ptr++ = AMF3_FALSE;

    return ptr;
}

uint8_t *amf3_write_true(uint8_t *ptr, const uint8_t *end)
{
    if (!ptr || ptr + 1 > end) {
        return NULL;
    }

    *ptr++ = AMF3_TRUE;

    return ptr;
}

uint8_t *amf3_write_integer(uint8_t *ptr, const uint8_t *end, uint32_t value)
{
    if (!ptr || ptr + 1 > end) {
        return NULL;
    }

    *ptr++ = AMF3_INTEGER;

    return amf3_write_int29(ptr, end, value);
}

uint8_t *amf3_write_double(uint8_t *ptr, const uint8_t *end, double value)
{
    uint8_t *double_ptr;

    if (!ptr || ptr + 9 > end) {
        return NULL;
    }

    double_ptr = (uint8_t *) &value;

    *ptr++ = AMF3_DOUBLE;

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

uint8_t *amf3_write_string(uint8_t *ptr, const uint8_t *end, const char *string, size_t length)
{
    if (!ptr || ptr + 1 > end || !string || length > 0x3FFFFFFF) {
        return NULL;
    }

    *ptr++ = AMF3_STRING;

    return amf3_write_string29(ptr, end, string, length);
}

uint8_t *amf3_write_xml_document(uint8_t *ptr, const uint8_t *end, const char *string, size_t length)
{
    if (!ptr || ptr + 1 > end || !string || length > 0x3FFFFFFF) {
        return NULL;
    }

    *ptr++ = AMF3_XML_DOCUMENT;

    return amf3_write_string29(ptr, end, string, length);
}

uint8_t *amf3_write_date(uint8_t *ptr, const uint8_t *end, double timestamp)
{
    if (!ptr || ptr + 10 > end) {
        return NULL;
    }

    *ptr = AMF3_DATE;

    amf3_write_int29(ptr + 1, end, 8 << 1 | 1);
    amf3_write_double(ptr + 2, end, timestamp);

    return ptr + 10;
}

uint8_t *amf3_write_array(uint8_t *ptr, const uint8_t *end, struct amf_object_item_t *item)
{
    int index;

    if (!ptr || ptr + 1 > end || !item || item->type != AMF3_ARRAY) {
        return NULL;
    }

    *ptr++ = AMF3_ARRAY;

    struct amf_object_item_t *associative_portion;

    AMF_CHECK_POINTER(item->value);
    associative_portion = item->value;
    for (index = 0; index < associative_portion->size; index++) {
        struct amf_object_item_t *current_item = ((struct amf_object_item_t *) associative_portion->value) + index;

        AMF_CHECK_POINTER(current_item->name);
        ptr = amf3_write_string29(ptr, end, current_item->name, strlen(current_item->name));

        ptr = amf3_write(ptr, end, associative_portion->value);
        AMF_CHECK_POINTER(ptr);
    }

    for (index = 1; index < item->size; index++) {
        ptr = amf3_write(ptr, end, ((struct amf_object_item_t *) item->value) + index);
        AMF_CHECK_POINTER(ptr);
    }

    return ptr;
}

uint8_t *amf3_write_object(uint8_t *ptr, const uint8_t *end, struct amf_object_item_t *item)
{
    int index;

    if (!ptr || ptr + 1 > end || !item || item->type != AMF3_ARRAY) {
        return NULL;
    }

    *ptr++ = AMF3_OBJECT;

    ptr = amf3_write_int29(ptr, end, 0x0B);
    AMF_CHECK_POINTER(ptr);

    AMF_CHECK_POINTER(item->name);
    ptr = amf3_write_string29(ptr, end, item->name, strlen(item->name));

    for (index = 1; index < item->size; index++) {
        struct amf_object_item_t *current_item = ((struct amf_object_item_t *) item->value) + index;

        AMF_CHECK_POINTER(current_item->name);
        ptr = amf3_write_string29(ptr, end, current_item->name, strlen(current_item->name));

        ptr = amf3_write(ptr, end, current_item);
        AMF_CHECK_POINTER(ptr);
    }

    return ptr;
}

uint8_t *amf3_write_xml(uint8_t *ptr, const uint8_t *end, const char *string, size_t length)
{
    if (!ptr || ptr + 1 > end || !string || length > 0x3FFFFFFF) {
        return NULL;
    }

    *ptr++ = AMF3_XML;

    return amf3_write_string29(ptr, end, string, length);
}

uint8_t *amf3_write_byte_array(uint8_t *ptr, const uint8_t *end, const char *string, size_t length)
{
    if (!ptr || ptr + 1 > end || !string || length > 0x3FFFFFFF) {
        return NULL;
    }

    *ptr++ = AMF3_BYTEARRAY;

    return amf3_write_string29(ptr, end, string, length);
}
