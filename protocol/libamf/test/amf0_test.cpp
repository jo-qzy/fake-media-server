//
// Created by BoringWednesday on 2021/8/3.
//

#include "amf0.h"

#include <cstring>
#include <fstream>
#include <iostream>

using namespace std;

#define AMF_OBJECT_ITEM_VALUE(item, amf_type, amf_name, amf_value, amf_size) \
    { item.type = amf_type; item.name = amf_name; item.value = amf_value; item.size = amf_size; }

struct rtmp_result_t
{
    char onStatus[64];
    double number;
    char level[64];
    char code[64];
    char description[256];
};

int amf0_test_write()
{
    rtmp_result_t result = {
            "onStatus",
            0,
            "status",
            "NetStream.Publish.Start",
            "Start publishing"
    };

    amf_object_item_t onStatus;
    amf_object_item_t number_item;
    amf_object_item_t null_item;
    amf_object_item_t object;
    amf_object_item_t item[3];

    uint8_t out_amf_buffer[1024], in_amf_buffer[1024];
    uint8_t *ptr, *end;

    ptr = out_amf_buffer;
    end = out_amf_buffer + 1024;

    AMF_OBJECT_ITEM_VALUE(onStatus, AMF0_STRING, nullptr, result.onStatus, 8);
    AMF_OBJECT_ITEM_VALUE(number_item, AMF0_NUMBER, nullptr, &result.number, 8);
    AMF_OBJECT_ITEM_VALUE(null_item, AMF0_NULL, nullptr, nullptr, 0);
    AMF_OBJECT_ITEM_VALUE(object, AMF0_OBJECT, nullptr, item, 3);
    AMF_OBJECT_ITEM_VALUE(item[0], AMF0_STRING, "level", result.level, 6);
    AMF_OBJECT_ITEM_VALUE(item[1], AMF0_STRING, "code", result.code, 23);
    AMF_OBJECT_ITEM_VALUE(item[2], AMF0_STRING, "description", result.description, 16);

    ptr = amf0_write(ptr, end, &onStatus);
    ptr = amf0_write(ptr, end, &number_item);
    ptr = amf0_write(ptr, end, &null_item);
    ptr = amf0_write(ptr, end, &object);
    if (!ptr) {
        cout << "Test write amf0 error." << endl;

        return -1;
    }

    fstream in_amf_stream("../test/rtmp_amf0_stream", ios::in);

    in_amf_stream.read(reinterpret_cast<char *>(in_amf_buffer), 1024);

    int ret = memcmp(in_amf_buffer, out_amf_buffer, ptr - out_amf_buffer);
    if (ret != 0) {
        cout << "Generate amf0 stream different from input amf0 stream." << endl;

        return -1;
    }

    return 0;
}

int amf0_test_read()
{
    return 0;
}

int main()
{
    int write_test, read_test;

    write_test = amf0_test_write();
    read_test = amf0_test_read();

    return (write_test != 0 || read_test != 0) ? -1 : 0;
}
