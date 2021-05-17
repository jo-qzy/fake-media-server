//
// Created by BoringWednesday on 2021/5/16.
//

#ifndef FAKE_MEDIA_SERVER_TCP_EVENT_H
#define FAKE_MEDIA_SERVER_TCP_EVENT_H


#include <net/event.h>
#include <util/io_buffer.h>

class Adapter;

class TCPEvent : public Event
{
public:
    explicit TCPEvent(IOLoop *loop, int fd = -1, bool listen_mode = false);
    ~TCPEvent() override;

    int on_read() override;
    int on_write() override;
    int send_handler(const void *data, size_t size);

    void set_protocol_adapter(Adapter *adapter);

private:
    bool        is_listener;
    IOBuffer    in_buffer;
    IOBuffer    out_buffer;
    Adapter    *adapter;

};


#endif //FAKE_MEDIA_SERVER_TCP_EVENT_H
