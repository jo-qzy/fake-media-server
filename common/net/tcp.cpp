//
// Created by BoringWednesday on 2021/5/16.
//

#include "tcp.h"
#include "adapter.h"
#include "socket_util.h"
#include "io_loop.h"
#include <util/log.h>
#include <util/define.h>

TCPEvent::TCPEvent(IOLoop *loop, int fd, bool listen_mode)
    : Event(loop, fd), is_listener(listen_mode)
    , adapter(nullptr)
{}

TCPEvent::~TCPEvent()
{
    close_socket(fd);
    io_loop->del_event(this);

    delete adapter;
}

int TCPEvent::on_read()
{
    if (is_listener) {
        int         client_sock;
        sockaddr    addr;
        socklen_t   addr_len;

        client_sock = accept_connection(fd, &addr, &addr_len);
        if (client_sock > 0) {

            TCPEvent *tcp_event = new TCPEvent(io_loop, client_sock);

            tcp_event->enable_read();
            adapter->accept(tcp_event);
        }

        return FMS_OK;
    }

    int read_size = in_buffer.read_from_event(this, nullptr, nullptr);

    if (read_size == 0) {
        return FMS_CLOSE;
    } else if (read_size == -1) {
        return FMS_AGAIN;
    }

    return adapter->read_handler(&in_buffer);
}

int TCPEvent::on_write()
{
    int status = out_buffer.write_to_event(this);

    if (out_buffer.empty()) {
        disable_write();
    }

    return status;
}

int TCPEvent::send_handler(const void *data, size_t size)
{
    int status = out_buffer.write_n(data, size);

    if (status == FMS_ERR) {
        LOG(ERROR) << "Adapter write to buffer error.";

        return FMS_ERR;
    }

    enable_write();

    return FMS_OK;
}

void TCPEvent::set_protocol_adapter(Adapter *protocol_adapter)
{
    adapter = protocol_adapter;
}
