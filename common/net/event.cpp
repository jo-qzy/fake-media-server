//
// Created by BoringWednesday on 2021/5/8.
//

#include "event.h"
#include "io_loop.h"
#include <sys/epoll.h>

Event::Event(IOLoop *io_loop, int fd)
    : fd(fd), events(0), is_add(false), io_loop(io_loop)
{}

Event::~Event() = default;

int Event::get_fd()
{
    return fd;
}

int Event::get_events()
{
    return events;
}

void Event::enable_read()
{
    if (events & EPOLLIN) {
        return;
    }

    events |= EPOLLIN;

    add_event();
}

void Event::disable_read()
{
    if (!(events & EPOLLIN)) {
        return;
    }

    events ^= EPOLLIN;

    del_event();
}

void Event::enable_write()
{
    if (events & EPOLLOUT) {
        return;
    }

    events |= EPOLLOUT;

    add_event();
}

void Event::disable_write()
{
    if (!(events & EPOLLOUT)) {
        return;
    }

    events ^= EPOLLOUT;

    del_event();
}

void Event::add_event()
{
    if (events == 0) {
        return;
    }

    int status = -1;

    if (is_add) {
        status = io_loop->add_event(this);
    } else {
        status = io_loop->mod_event(this);
    }

    is_add = status == 0 || is_add;
}

void Event::del_event()
{
    if (!is_add) {
        return;
    }

    int status = -1;

    if (events == 0) {
        status = io_loop->del_event(this);
    } else {
        status = io_loop->mod_event(this);
    }

    is_add = status != 0 || events != 0;
}
