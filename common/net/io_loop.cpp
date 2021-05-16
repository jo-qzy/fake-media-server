//
// Created by BoringWednesday on 2021/5/13.
//

#include "io_loop.h"
#include "event.h"
#include <util/log.h>
#include <unistd.h>
#include <sys/epoll.h>

bool IOLoop::interrupt = false;

IOLoop::IOLoop(int max_connections, int interval)
    : poll_id(-1), poll_timeout(interval)
    , event_ptr(nullptr)
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
    if (poll_id <= 0) {
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

    event.events = ev->get_events();
    event.data.ptr = (void *) ev;

    if (epoll_ctl(poll_id, EPOLL_CTL_ADD, ev->get_fd(), &event) < 0) {
        LOG(ERROR) << "IOLoop add event failed, fd: " << ev->get_fd()
            << ", errno: " << errno;

        return -1;
    }

    return 0;
}

int IOLoop::del_event(Event *ev)
{
    if (epoll_ctl(poll_id, EPOLL_CTL_DEL, ev->get_fd(), nullptr) < 0) {
        LOG(ERROR) << "IOLoop delete event failed, fd: " << ev->get_fd()
                   << ", errno: " << errno;

        return -1;
    }

    return 0;
}

int IOLoop::mod_event(Event *ev)
{
    epoll_event event;

    event.events = ev->get_events();
    event.data.ptr = (void *) ev;

    if (epoll_ctl(poll_id, EPOLL_CTL_MOD, ev->get_fd(), &event) < 0) {
        LOG(ERROR) << "IOLoop modify event failed, fd: " << ev->get_fd()
                   << ", errno: " << errno;

        return -1;
    }

    return 0;
}

void IOLoop::event_loop()
{
    int             ret = 0;
    epoll_event    *events = (epoll_event *) event_ptr;

    while (!interrupt) {
        ret = epoll_wait(poll_id, events, event_num, 1000);

        if (ret <= 0) {
            DLOG(INFO) << "IOLoop detected noting, ret: " << ret
                << ", errno: " << errno;
        }
    }
}