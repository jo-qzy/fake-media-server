//
// Created by BoringWednesday on 2021/7/28.
//

#include <stdio.h>
#include "amf3.h"

int main()
{
    uint8_t buffer[256];

    uint8_t *ptr;

    ptr = amf3_write_string(buffer, buffer + 256, "hello", 5);

    printf("%s\n", buffer);
    if (ptr == NULL) {
        printf("test\n");
    }

    return 0;
}
