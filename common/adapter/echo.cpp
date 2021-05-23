//
// Created by BoringWednesday on 2021/5/23.
//

#include "echo.h"
#include "net/tcp.h"
#include "util/define.h"
#include "util/log.h"
#include <echo_server.h>

EchoAdapter::EchoAdapter(Event *ev)
    : Adapter(ev), echo_server(nullptr)
{}

EchoAdapter::~EchoAdapter()
{
    echo_server_destroy(echo_server);
}

int EchoAdapter::connect()
{
    return FMS_OK;
}

int EchoAdapter::accept(Event *ev)
{
    EchoAdapter    *echo;
    TCPEvent       *tcp_ev;

    echo = new EchoAdapter(ev);
    echo->init_echo_protocol();
    tcp_ev = dynamic_cast<TCPEvent *>(ev);
    tcp_ev->set_protocol_adapter(echo);
    tcp_ev->enable_read();

    return FMS_OK;
}

int EchoAdapter::read_handler(IOBuffer *buffer)
{
    int read_status;

    read_status = echo_server_input(echo_server, buffer->data(), buffer->size());

    buffer->resize();

    if (read_status <= 0) {
        return FMS_ERR;
    }

    return FMS_OK;
}

int EchoAdapter::init_echo_protocol()
{
    if (echo_server) {
        return FMS_OK;
    }

    echo_server_handler_t handler;

    handler.send = echo_send;

    echo_server = echo_server_create(this, &handler);
    if (echo_server == nullptr) {
        return FMS_ERR;
    }

    return 0;
}

int EchoAdapter::echo_send(void *param, const void *data, size_t size)
{
    EchoAdapter    *echo_adapter = (EchoAdapter *) param;
    TCPEvent       *tcp_ev = (TCPEvent *) echo_adapter->event;

    return tcp_ev->send_handler(data, size);
}
