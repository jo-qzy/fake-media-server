//
// Created by BoringWednesday on 2021/5/9.
//

#include "fake_media_server.h"
#include "log.h"
#include "worker.h"
#include <csignal>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

bool FakeMediaServer::interrupt = false;

FakeMediaServer::FakeMediaServer(const char *conf_path)
    : master_mode(true), worker_flag(false)
{
    this->conf_path = conf_path;
}

void FakeMediaServer::Run()
{
    pid_t child_pid;

    // TODO: parse conf
    master_mode = true;
    int worker_num = 2;

    LOG(INFO) << "Fake Media Server is online.";

    for (int i = 1; i <= worker_num; i++) {
        if (!CreateWorker(i)) {
            interrupt = true;
            master_mode = true;

            break;
        }

        if (worker_flag) {
            return;
        }
    }

    if (!master_mode) {
        LOG(INFO) << "Fake Media Server master mode is off, master begin quit.";

        return;
    }

    if (!RegisterSignal()) {
        return;
    }

    while (!interrupt) {
        child_pid = waitpid(-1, nullptr, WNOHANG);
        if (child_pid != 0) {
            auto it = worker_map.find(child_pid);
            if (it != worker_map.end()) {
                LOG(FATAL) << "worker no." << it->second << " is down.";
                LOG(WARN) << "try to restart worker no." << it->second << "...";

                /* Delete down worker */
                worker_map.erase(child_pid);

                /* Restart new worker */
                if (!CreateWorker(it->second)) {
                    LOG(FATAL) << "try to restart worker failed.";
                }
            }
        }

        sleep(1);
    }

    StopAllWorker();
}

void FakeMediaServer::Stop()
{
    interrupt = true;
}

void FakeMediaServer::StopAllWorker()
{
    for (auto it: worker_map) {
        kill(it.first, SIGRTMIN + 1);
    }

    /* No need to call waitpid() */

    worker_map.clear();
}

bool FakeMediaServer::CreateWorker(int serial)
{
    pid_t pid = fork();

    if (pid < 0) {
        return false;
    } else if (pid == 0) {
        /* Create worker and run loop */
        Worker worker;

        worker_flag = true;

        worker.Run();

        LOG(WARN) << "worker no." << serial << " is ready to quit...";
    } else {
        worker_map[pid] = serial;
    }

    return true;
}

bool FakeMediaServer::RegisterSignal()
{
    if (signal(SIGINT, SignalHandler) == SIG_ERR) {
        LOG(ERROR) << "master register signal handler failed.";

        return false;
    }

    return true;
}

void FakeMediaServer::SignalHandler(int signal)
{
    LOG(INFO) << "Fake Media Server receive SIGINT...";

    Stop();
}
