//
// Created by BoringWednesday on 2021/7/7.
//

#ifndef LIBAMF_AMF3_H
#define LIBAMF_AMF3_H

#include <stdint.h>

#include "amf_item.h"

#ifdef __cplusplus
extern "C" {
#endif

uint8_t *amf3_write(uint8_t *ptr, const uint8_t *end, struct amf_object_item_t *item);

#ifdef __cplusplus
}
#endif

#endif //LIBAMF_AMF3_H
