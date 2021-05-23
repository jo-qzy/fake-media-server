//
// Created by BoringWednesday on 2021/5/9.
//

#include "core.h"
#include "worker.h"
#include "adapter/echo.h"
#include "net/socket_util.h"
#include "net/tcp.h"
#include "util/process.h"
#include <signal.h>

Worker *Worker::worker_instance = nullptr;

Worker * Worker::create_worker(int max_events)
{
    if (worker_instance == nullptr) {
        worker_instance = new Worker(max_events);

        if (worker_instance->create() != 0) {
            delete worker_instance;
            worker_instance = nullptr;

            return nullptr;
        }
    }

    return worker_instance;
}

Worker * Worker::get_worker()
{
    if (worker_instance == nullptr) {
        return create_worker(1024);
    }

    return worker_instance;
}

Worker::Worker(int max_events)
    : IOLoop(max_events)
{
    if (register_signal(SIGQUIT, signal_handler) != 0) {
        interrupt = true;
    }

    if (register_signal(SIGINT, SIG_DFL) != 0) {
        interrupt = true;
    }
}

Worker::~Worker()
{}

int Worker::initial_event()
{
    // TODO: base on setting to create event
    TCPEvent *tcp_ev;
    EchoAdapter *echo;

    int sock = create_tcp_socket_nonblock();
    if (bind_and_listen(sock, "0.0.0.0", 8888, 1024) != 0) {
        return -1;
    }

    tcp_ev = new TCPEvent(this, sock, true);
    echo = new EchoAdapter(tcp_ev);

    tcp_ev->set_protocol_adapter(echo);
    tcp_ev->enable_read();

    return 0;
}

void Worker::signal_handler(int signal)
{
    if (signal == SIGQUIT) {
        LOG(INFO) << "Fake Media Server worker receive SIGQUIT.";
    }

    stop();
}
