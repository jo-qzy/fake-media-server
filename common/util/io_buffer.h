//
// Created by BoringWednesday on 2021/5/12.
//

#ifndef FAKE_MEDIA_SERVER_IO_BUFFER_H
#define FAKE_MEDIA_SERVER_IO_BUFFER_H


#include <stdint.h>
#include <stddef.h>
#include <sys/socket.h>

class Event;

class IOBuffer
{
public:
    explicit IOBuffer(int capacity = 2 * 1024);
    ~IOBuffer();

    int read_from_event(Event *ev, sockaddr *addr, socklen_t *addr_len);
    int read_n(void *data, size_t size);

    int write_to_event(Event *ev);
    int write_n(const void *data, size_t size);

    uint8_t * data();
    void resize();

    bool empty();
    int size();

private:
    int space_prepare(size_t size);
    void in_rewind(size_t size);
    void out_rewind(size_t size);
    void rewind(size_t size, bool in);

private:
    uint8_t    *buffer;
    uint8_t    *start;
    uint8_t    *end;
    uint32_t    data_size;
    uint32_t    buffer_size;

};


#endif //FAKE_MEDIA_SERVER_IO_BUFFER_H
