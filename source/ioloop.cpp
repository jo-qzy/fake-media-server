//
// Created by BoringWednesday on 2021/5/13.
//

#include "ioloop.h"
#include "core.h"
#include <unistd.h>
#include <csignal>
#include <memory>
#include <sys/epoll.h>

using namespace std;

IOLoop::IOLoop(int max_connections)
    : poll_id(-1), event_ptr(nullptr)
    , event_num(max_connections), interrupt(false)
{}

IOLoop::~IOLoop()
{}

void IOLoop::run()
{
    poll_id = epoll_create(1);
    if (poll_id != 0) {
        return;
    }

    event_ptr = new epoll_event[event_num];

    event_loop();

    close(poll_id);
    delete[] (epoll_event *) event_ptr;
}

void IOLoop::stop()
{
    interrupt = true;
}

void IOLoop::event_loop()
{
    epoll_event *events;

    events = (epoll_event *) event_ptr;

    while (!interrupt) {
        epoll_wait(poll_id, events, event_num, 100);
        LOG(INFO) << "nothing";
    }
}
