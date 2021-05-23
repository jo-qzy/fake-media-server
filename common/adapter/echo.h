//
// Created by BoringWednesday on 2021/5/23.
//

#ifndef FAKE_MEDIA_SERVER_ECHO_H
#define FAKE_MEDIA_SERVER_ECHO_H

#include <cstddef>
#include "net/adapter.h"

struct echo_server_t;
class IOBuffer;

class EchoAdapter : public Adapter
{
public:
    explicit EchoAdapter(Event *ev);
    ~EchoAdapter() override;

    int connect() override;
    int accept(Event *ev) override;
    int read_handler(IOBuffer *buffer) override;

private:
    int init_echo_protocol();

    static int echo_send(void *param, const void * data, size_t size);

private:
    echo_server_t  *echo_server;
};


#endif //FAKE_MEDIA_SERVER_ECHO_H
