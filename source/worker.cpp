//
// Created by BoringWednesday on 2021/5/9.
//

#include "core.h"
#include "worker.h"
#include <unistd.h>
#include <csignal>
#include <memory>
#include <sys/epoll.h>

using namespace std;

bool Worker::interrupt = false;

Worker::Worker()
{}

void Worker::run()
{
    if (!register_signal()) {
        return;
    }

    event_loop();
}

void Worker::stop()
{
    interrupt = true;
}

void Worker::event_loop()
{
    int epoll_id;
    unique_ptr<epoll_event> events(new epoll_event[1024]);

    epoll_id = epoll_create(1);
    if (epoll_id != 0) {
        return;
    }

    while (!interrupt) {
        epoll_wait(epoll_id, events.get(), 1024, 100);
    }
}

bool Worker::register_signal()
{
    if (signal(SIGQUIT, signal_handler) == SIG_ERR) {
        LOG(ERROR) << "worker register signal handler failed.";

        return false;
    }

    if (signal(SIGINT, SIG_DFL) == SIG_ERR) {
        LOG(ERROR) << "worker register signal handler failed.";

        return false;
    }

    return true;
}

void Worker::signal_handler(int signal)
{
    if (signal == SIGQUIT) {
        LOG(INFO) << "Fake Media Server worker receive SIGQUIT.";
    }

    stop();
}
