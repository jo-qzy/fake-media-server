//
// Created by BoringWednesday on 2021/7/27.
//

#ifndef LIBAMF_ITEM_H
#define LIBAMF_ITEM_H

#include <stddef.h>

struct amf_object_item_t
{
    int         type;
    const char *name;
    void       *value;
    size_t      size;
};

#endif //LIBAMF_ITEM_H
