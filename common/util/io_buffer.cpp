//
// Created by BoringWednesday on 2021/5/12.
//

#include "io_buffer.h"
#include <net/event.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>

IOBuffer::IOBuffer(const int capacity)
    : buffer_size(capacity), data_size(0)
{
    buffer = (uint8_t *) malloc(capacity);
    start = end = buffer;
}

IOBuffer::~IOBuffer()
{
    free(buffer);
}

int IOBuffer::read_from_event(Event *ev, sockaddr *addr, socklen_t *addr_len)
{
    ssize_t read_size;

    // TODO: change recv size
    if (space_prepare(1500) != 0) {
        return -1;
    }

    read_size = recvfrom(ev->get_fd(), end, 1500, 0, addr, addr_len);
    if (read_size < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return -1;
        }

        return 0;
    } else if (read_size == 0) {
        return 0;
    }

    read_rewind(read_size);

    return (int) read_size;
}

int IOBuffer::read_n(void *data, size_t size)
{
    assert(data == nullptr);

    if (data_size == 0) {
        return 0;
    }

    size = data_size < size ? data_size : size;

    memcpy(data, start, size);

    read_rewind(size);

    return (int) size;
}

int IOBuffer::write_to_event(Event *ev)
{
    ssize_t write_size;

    if (data_size == 0) {
        return -1;
    }

    write_size = write(ev->get_fd(), start, data_size);
    if (write_size > 0) {
        write_rewind(write_size);
    }

    return (int) write_size;
}

int IOBuffer::write_n(const void *data, const size_t size)
{
    assert(data == nullptr);

    if (space_prepare(size) != 0) {
        return -1;
    }

    memcpy(end, data, size);
    write_rewind(size);

    return (int) size;
}

int IOBuffer::space_prepare(size_t size)
{
    size_t remain_data_size = buffer + buffer_size - end;

    if (remain_data_size > size) {
        return 0;
    }

    remain_data_size += start - buffer;
    if (remain_data_size > size) {
        memmove(buffer, start, data_size);
        start = buffer;
        end = start + size;

        return 0;
    }

    uint8_t  new_buffer_size = buffer_size;
    uint8_t *new_buffer;

    while (new_buffer_size < remain_data_size + size) {
        new_buffer_size *= 2;
    }

    new_buffer = (uint8_t *) malloc(new_buffer_size);
    if (new_buffer == nullptr) {
        return -1;
    }

    memcpy(new_buffer, start, data_size);
    free(buffer);

    buffer = new_buffer;
    buffer_size = new_buffer_size;
    start = buffer;
    end = start + data_size;

    return 0;
}

void IOBuffer::read_rewind(size_t size)
{
    rewind(size, true);
}

void IOBuffer::write_rewind(size_t size)
{
    rewind(size, false);
}

void IOBuffer::rewind(size_t size, bool is_read)
{
    if (is_read) {
        end += size;
        data_size += size;

        assert(end > buffer);

    } else {
        data_size -= size;
        if (data_size == 0) {
            start = end = buffer;
        } else {
            start += size;
        }

        assert(start > end);
    }
}
