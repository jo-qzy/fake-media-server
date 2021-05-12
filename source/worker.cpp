//
// Created by BoringWednesday on 2021/5/9.
//

#include "core.h"
#include "worker.h"
#include <unistd.h>
#include <csignal>

bool Worker::interrupt = false;

void Worker::run()
{
    // if (!rename_worker()) {
    //
    // }

    if (!register_signal()) {
        return;
    }

    while (!interrupt) {
        LOG(INFO) << "Worker run...";
        sleep(1);
    }
}

void Worker::stop()
{
    interrupt = true;
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
