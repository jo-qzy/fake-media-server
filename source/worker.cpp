//
// Created by BoringWednesday on 2021/5/9.
//

#include "worker.h"
#include "log.h"
#include <unistd.h>

bool Worker::interrupt = false;

void Worker::Run()
{
    if (!RegisterSignal()) {
        return;
    }

    while (!interrupt) {
        LOG(INFO) << "Worker run...";
        sleep(1);
    }
}

void Worker::Stop()
{
    interrupt = true;
}

bool Worker::RegisterSignal()
{
    if (signal(SIGRTMIN + 1, SignalHandler) == SIG_ERR) {
        LOG(ERROR) << "worker register signal handler failed.";

        return false;
    }

    if (signal(SIGINT, SIG_IGN) == SIG_ERR) {
        LOG(ERROR) << "worker register signal handler failed.";

        return false;
    }

    return true;
}

void Worker::SignalHandler(int signal)
{
    LOG(INFO) << "Fake Media Server worker receive SIGRTMIN + 1...";

    Stop();
}
