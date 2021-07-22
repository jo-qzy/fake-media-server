//
// Created by BoringWednesday on 2021/7/22.
//

#ifndef LIBAMF_AMF_UTIL_H
#define LIBAMF_AMF_UTIL_H

#include <stdint.h>
#include <stddef.h>

uint8_t *amf0_write_int16(uint8_t *ptr, const uint8_t *end, uint16_t value);
uint8_t *amf0_write_int32(uint8_t *ptr, const uint8_t *end, uint32_t value);
uint8_t *amf0_write_string16(uint8_t *ptr, const uint8_t *end, const char *str, size_t length);
uint8_t *amf0_write_string32(uint8_t *ptr, const uint8_t *end, const char *str, size_t length);

#endif //LIBAMF_AMF_UTIL_H
