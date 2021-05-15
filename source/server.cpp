//
// Created by BoringWednesday on 2021/5/9.
//

#include "core.h"
#include "server.h"
#include "worker.h"
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

bool FakeMediaServer::interrupt = false;

FakeMediaServer::FakeMediaServer(const char *conf_path)
    : worker_flag(false), server_conf(nullptr)
{
    this->conf_path = conf_path;
}

void FakeMediaServer::run()
{
    pid_t       child_pid;
    MainConf   *main_conf;

    if (ServerConf::parse_conf(conf_path) != 0) {
        LOG(ERROR) << "parse conf error";

        return;
    }

    server_conf = ServerConf::get_server_conf();
    if (server_conf == nullptr) {
        LOG(ERROR) << "parse conf " << conf_path << " failed";

        return;
    }

    LOG(INFO) << "Fake Media Server is online.";

    main_conf = ServerConf::get_main_conf();

    /* master mode is off, worker reset into 1 */
    if (!main_conf->master_mode) {
        main_conf->worker_num = 1;
    }

    for (int i = 1; i <= main_conf->worker_num; i++) {
        if (!create_worker(i)) {
            interrupt = true;
            main_conf->master_mode = true;

            break;
        }

        if (worker_flag) {
            LOG(WARN) << "Fake Media Server ready to exit.";

            return;
        }
    }

    if (!main_conf->master_mode) {
        LOG(INFO) << "Fake Media Server master mode is off, master begin quit.";

        return;
    }

    if (!register_signal()) {
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
                if (!create_worker(it->second)) {
                    LOG(FATAL) << "try to restart worker failed.";
                }
            }
        }

        sleep(1);
    }

    stop_worker();
}

void FakeMediaServer::stop()
{
    interrupt = true;
}

void FakeMediaServer::stop_worker()
{
    for (auto it: worker_map) {
       kill(it.first, SIGQUIT);
    }

    /* No need to call waitpid() */

    worker_map.clear();
}

bool FakeMediaServer::create_worker(int serial)
{
    MainConf *main_conf = ServerConf::get_main_conf();

    if (!main_conf->master_mode) {
        Worker *worker = Worker::create_worker(1024);

        worker_flag = true;
        worker->run();

        return true;
    }

    pid_t pid = fork();

    if (pid < 0) {
        return false;
    } else if (pid == 0) {
        /* Create worker and run loop */
        Worker *worker = Worker::create_worker(1024);

        worker_flag = true;
        worker->run();

        LOG(WARN) << "worker no." << serial << " is ready to quit...";

    } else {
        /* Add Worker IOLoop pid into worker_map, so we can manage it */
        worker_map[pid] = serial;
    }

    return true;
}

bool FakeMediaServer::register_signal()
{
    if (signal(SIGINT, signal_handler) == SIG_ERR) {
        LOG(ERROR) << "master register signal handler failed.";

        return false;
    }

    return true;
}

void FakeMediaServer::signal_handler(int signal)
{
    LOG(INFO) << "Fake Media Server receive SIGINT...";

    stop();
}
