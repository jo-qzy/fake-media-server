//
// Created by BoringWednesday on 2021/7/7.
//

#include <assert.h>
#include <string.h>

#include "amf0.h"
#include "amf3.h"
#include "amf_util.h"

/*
 * AMF Format:
 *      AMF_TYPE: 1 byte
 *      DATA_LENGTH: 0/2/4 bytes
 *      DATA: N bytes
 */

uint8_t *amf0_write(uint8_t *ptr, const uint8_t *end, struct amf_object_item_t *item)
{
    if (!ptr || !item) {
        return NULL;
    }

    switch (item->type) {
        case AMF0_NUMBER:
            AMF_CHECK_POINTER(item->value);

            return amf0_write_number(ptr, end, *(double *) item->value);
        case AMF0_BOOLEAN:
            AMF_CHECK_POINTER(item->value);

            return amf0_write_boolean(ptr, end, *(uint8_t *) item->value);
        case AMF0_STRING:
            AMF_CHECK_POINTER(item->value);

            return amf0_write_string(ptr, end, item->value, strlen(item->value));
        case AMF0_OBJECT:
            return amf0_write_object(ptr, end, item->value);
        case AMF0_MOVIE_CLIP:
            return NULL;
        case AMF0_NULL:
            return amf0_write_null(ptr, end);
        case AMF0_UNDEFINED:
            return amf0_write_undefined(ptr, end);
        case AMF0_REFERENCE:
            AMF_CHECK_POINTER(item->value);

            return amf0_write_reference(ptr, end, *(uint16_t *) item->value);
        case AMF0_ECMA_ARRAY:
            return amf0_write_ecma_array(ptr, end, item->value);
        case AMF0_OBJECT_END:
            return amf0_write_object_end(ptr, end);
        case AMF0_STRICT_ARRAY:
            return amf0_write_strict_array(ptr, end, item->value);
        case AMF0_DATE:
            AMF_CHECK_POINTER(item->value);

            // Use item->value = milliseconds, item->size = timezone
            return amf0_write_date(ptr, end, *(double *) item->value, (int16_t) item->size);
        case AMF0_LONG_STRING:
            AMF_CHECK_POINTER(item->value);

            return amf0_write_long_string(ptr, end, item->value, strlen(item->value));
        case AMF0_UNSUPPORTED:
        case AMF0_RECORDSET:
            return NULL;
        case AMF0_XML_DOCUMENT:
            return amf0_write_xml_document(ptr, end, item->value);
        case AMF0_TYPED_OBJECT:
            return amf0_write_typed_object(ptr, end, item->value);
        case AMF0_AVMPLUS_OBJECT:
            return amf0_write_avmplus_object(ptr, end, item->value);
        default:
            return NULL;
    }
}

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

    return amf_write_string16(ptr, end, string, length);
}

uint8_t *amf0_write_object(uint8_t *ptr, const uint8_t *end, struct amf_object_item_t *item)
{
    int index;

    if (!ptr || ptr + 1 > end || !item || item->type != AMF0_OBJECT) {
        return NULL;
    }

    *ptr++ = AMF0_OBJECT;

    for (index = 0; index < item->size; index++) {
        struct amf_object_item_t *current_item = ((struct amf_object_item_t *) item->value) + index;

        ptr = amf_write_string16(ptr, end, current_item->name, strlen(current_item->name));
        if (!ptr) {
            return NULL;
        }

        ptr = amf0_write(ptr, end, current_item);
        if (!ptr) {
            return NULL;
        }
    }

    return amf0_write_object_end(ptr, end);
}

uint8_t *amf0_write_movie_clip(uint8_t *ptr, const uint8_t *end)
{
    // Nonsupport AMF type

    return NULL;
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

    return amf_write_int16(ptr, end, reference);
}

uint8_t *amf0_write_ecma_array(uint8_t* ptr, const uint8_t* end, struct amf_object_item_t *item)
{
    int index;

    if (!ptr || ptr + 5 > end || !item || item->type != AMF0_ECMA_ARRAY) {
        return NULL;
    }

    *ptr++ = AMF0_ECMA_ARRAY;

    // associative-count: 4 bytes
    ptr = amf_write_int32(ptr, end, item->size);

    for (index = 0; index < item->size; index++) {
        struct amf_object_item_t *current_item = ((struct amf_object_item_t *) item->value) + index;

        ptr = amf_write_string16(ptr, end, current_item->name, strlen(current_item->name));
        AMF_CHECK_POINTER(ptr);

        ptr = amf0_write(ptr, end, current_item);
        AMF_CHECK_POINTER(ptr);
    }

    return amf0_write_object_end(ptr, end);
}

uint8_t *amf0_write_object_end(uint8_t* ptr, const uint8_t* end)
{
    if (!ptr || ptr + 3 > end) {
        return NULL;
    }

    // End of Object: 0x00 0x00 0x09
    *ptr++ = 0;
    *ptr++ = 0;
    *ptr++ = AMF0_OBJECT_END;

    return ptr;
}

uint8_t *amf0_write_strict_array(uint8_t *ptr, const uint8_t *end, struct amf_object_item_t *item)
{
    int index;

    if (!ptr || ptr + 5 > end || !item || item->type != AMF0_STRICT_ARRAY) {
        return NULL;
    }

    *ptr++ = AMF0_STRICT_ARRAY;

    // associative-count: 4 bytes
    ptr = amf_write_int32(ptr, end, item->size);

    for (index = 0; index < item->size; index++) {
        struct amf_object_item_t *current_item = ((struct amf_object_item_t *) item->value) + index;

        ptr = amf0_write(ptr, end, current_item);
        AMF_CHECK_POINTER(ptr);
    }

    return ptr;
}

uint8_t *amf0_write_date(uint8_t *ptr, const uint8_t *end, double milliseconds, int16_t timezone)
{
    if (!ptr || ptr + 11 > end) {
        return NULL;
    }

    *ptr = AMF0_DATE;

    amf0_write_number(ptr + 1, end, milliseconds);

    amf_write_int16(ptr + 9, end, timezone);

    return ptr + 11;
}

uint8_t *amf0_write_long_string(uint8_t *ptr, const uint8_t *end, const char *string, size_t length)
{
    if (!ptr || length > UINT32_MAX || ptr + 1 + 4 + length > end) {
        return NULL;
    }

    *ptr++ = AMF0_LONG_STRING;

    return amf_write_string32(ptr, end, string, length);
}

uint8_t *amf0_write_unsupported(uint8_t *ptr, const uint8_t *end)
{
    return NULL;
}

uint8_t *amf0_write_recordset(uint8_t *ptr, const uint8_t *end)
{
    return NULL;
}

uint8_t *amf0_write_xml_document(uint8_t *ptr, const uint8_t *end, struct amf_object_item_t *item)
{
    if (!ptr || ptr + 1 > end) {
        return NULL;
    }

    *ptr++ = AMF0_XML_DOCUMENT;

    return amf0_write(ptr, end, (struct amf_object_item_t *) item->value);
}

uint8_t *amf0_write_typed_object(uint8_t *ptr, const uint8_t *end, struct amf_object_item_t *item)
{
    int index;

    if (!ptr || ptr + 1 > end) {
        return NULL;
    }

    *ptr++ = AMF0_TYPED_OBJECT;

    ptr = amf_write_string16(ptr, end, item->name, strlen(item->name));
    AMF_CHECK_POINTER(ptr);

    for (index = 0; index < item->size; index++) {
        struct amf_object_item_t *current_item = ((struct amf_object_item_t *) item->value) + index;

        ptr = amf_write_string16(ptr, end, current_item->name, strlen(current_item->name));
        AMF_CHECK_POINTER(ptr);

        ptr = amf0_write(ptr, end, current_item);
        AMF_CHECK_POINTER(ptr);
    }

    return amf0_write_object_end(ptr, end);
}

uint8_t *amf0_write_avmplus_object(uint8_t *ptr, const uint8_t *end, struct amf_object_item_t *item)
{
    if (!ptr || ptr + 1 > end) {
        return NULL;
    }

    *ptr++ = AMF0_AVMPLUS_OBJECT;

    return amf3_write(ptr, end, item);
}
