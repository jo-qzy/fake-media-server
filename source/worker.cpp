//
// Created by BoringWednesday on 2021/5/9.
//

#include "core.h"
#include "worker.h"
#include "util/process.h"
#include <csignal>
#include <unistd.h>

using namespace std;

Worker::Worker()
{}

Worker::~Worker()
{
    delete io_loop;
}

void Worker::run()
{
    if (register_signal(SIGQUIT, signal_handler) != 0) {
        return;
    }

    if (register_signal(SIGINT, SIG_DFL) != 0) {
        return;
    }

    io_loop = new IOLoop(1024);
    io_loop->run();
}

void Worker::signal_handler(int signal)
{
    if (signal == SIGQUIT) {
        LOG(INFO) << "Fake Media Server worker receive SIGQUIT.";
    }
}
