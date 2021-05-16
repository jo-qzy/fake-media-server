//
// Created by BoringWednesday on 2021/5/16.
//

#include "tcp.h"
#include "adapter.h"
#include "socket_util.h"

TCPEvent::TCPEvent(IOLoop *loop, int fd, bool listen_mode)
    : Event(loop, fd), is_listener(listen_mode)
    , adapter(nullptr)
{}

TCPEvent::~TCPEvent()
{
    close_socket(fd);
}

int TCPEvent::on_read()
{
    if (is_listener) {
        int         client_sock;
        sockaddr    addr;
        socklen_t   addr_len;

        client_sock = accept_connection(fd, &addr, &addr_len);
        if (client_sock < 0) {
            return -1;
        }

        TCPEvent *tcp_event = new TCPEvent(io_loop, client_sock);
        if (adapter->accept(tcp_event) != 0) {
            delete tcp_event;

            return -1;
        }

        tcp_event->enable_read();

        return 1;
    }

    int read_size = in_buffer.read_from_event(this, nullptr, nullptr);

    if (read_size <= 0) {
        return read_size;
    }

    return adapter->read_handler();
}

int TCPEvent::on_write()
{
    return out_buffer.write_to_event(this);
}

int TCPEvent::send_handler(const void *data, size_t size)
{
    return out_buffer.write_n(data, size);
}

void TCPEvent::set_protocol_adapter(Adapter *adapter)
{
    this->adapter = adapter;
}
