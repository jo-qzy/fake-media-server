//
// Created by BoringWednesday on 2021/5/13.
//

#include "ioloop.h"
#include "core.h"
#include "event.h"
#include <unistd.h>
#include <signal.h>
#include <sys/epoll.h>

bool IOLoop::interrupt = false;

IOLoop::IOLoop(int max_connections)
    : poll_id(-1), event_ptr(nullptr)
    , event_num(max_connections)
{}

IOLoop::~IOLoop()
{
    close(poll_id);
    delete[] (epoll_event *) event_ptr;
}

int IOLoop::create()
{
    poll_id = epoll_create(1);
    if (poll_id != 0) {
        return -1;
    }

    event_ptr = new epoll_event[event_num];

    return 0;
}

void IOLoop::run()
{
    event_loop();
}

void IOLoop::stop()
{
    interrupt = true;
}

int IOLoop::add_event(Event *ev)
{
    epoll_event event;

    return 0;
}

int IOLoop::del_event(Event *ev)
{
    return 0;
}

int IOLoop::mod_event(Event *ev)
{
    return 0;
}

void IOLoop::event_loop()
{
    epoll_event *events = (epoll_event *) event_ptr;

    while (!interrupt) {
        epoll_wait(poll_id, events, event_num, 100);
        LOG(INFO) << "nothing";
    }
}
