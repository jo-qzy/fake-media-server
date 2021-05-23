//
// Created by BoringWednesday on 2021/5/13.
//

#include "io_loop.h"
#include "event.h"
#include <assert.h>
#include <util/log.h>
#include <util/define.h>
#include <unistd.h>
#include <sys/epoll.h>

bool IOLoop::interrupt = false;

IOLoop::IOLoop(int max_connections, int interval)
    : poll_id(-1), poll_timeout(interval)
    , event_ptr(nullptr)
    , event_max_num(max_connections)
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

    event_ptr = new epoll_event[event_max_num];

    initial_event();

    return 0;
}

int IOLoop::initial_event()
{
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

    DLOG(INFO) << "IOLoop add fd: " << ev->get_fd()
        << ", event: " << ev->get_events();

    if (epoll_ctl(poll_id, EPOLL_CTL_ADD, ev->get_fd(), &event) < 0) {
        LOG(ERROR) << "IOLoop add event failed, fd: " << ev->get_fd()
            << ", errno: " << errno;

        return -1;
    }

    return 0;
}

int IOLoop::del_event(Event *ev)
{
    DLOG(INFO) << "IOLoop delete fd: " << ev->get_fd();

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

    DLOG(INFO) << "IOLoop modify fd: " << ev->get_fd()
               << ", event: " << ev->get_events();

    if (epoll_ctl(poll_id, EPOLL_CTL_MOD, ev->get_fd(), &event) < 0) {
        LOG(ERROR) << "IOLoop modify event failed, fd: " << ev->get_fd()
                   << ", errno: " << errno;

        return -1;
    }

    return 0;
}

void IOLoop::event_loop()
{
    int             event_num = 0, i = 0, status = 0;
    epoll_event    *events = (epoll_event *) event_ptr;
    Event          *ev;

    while (!interrupt) {
        event_num = epoll_wait(poll_id, events, event_max_num, 1000);

        if (event_num <= 0) {
            //DLOG(INFO) << "IOLoop detected noting, ret: " << event_num
            //    << ", errno: " << errno;

            continue;
        }

        for (i = 0; i < event_num; i++) {
            ev = (Event *) (events[i].data.ptr);

            assert(ev != nullptr);

            if (events[i].events & (EPOLLIN | EPOLLHUP)) {
                DLOG(INFO) << "Event fd: " << ev->get_fd()
                    << " trigger read event " << events[i].events;

                status = ev->on_read();
                if (status == FMS_ERR || status == FMS_CLOSE) {
                    DLOG(INFO) << "Delete event";

                    delete ev;

                    continue;
                }
            }

            if (events[i].events & EPOLLOUT) {
                DLOG(INFO) << "Event fd: " << ev->get_fd()
                           << " trigger write event.";

                status = ev->on_write();
                if (status == FMS_ERR || status == FMS_CLOSE) {
                    DLOG(INFO) << "Delete event";

                    delete ev;
                }
            }

        }

    }
}
