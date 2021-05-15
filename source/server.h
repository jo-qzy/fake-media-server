//
// Created by BoringWednesday on 2021/5/9.
//

#ifndef FAKE_MEDIA_SERVER_H
#define FAKE_MEDIA_SERVER_H

#include "conf.h"
#include <util/config.h>
#include <map>
#include <string>
#include <csignal>

using namespace std;

class FakeMediaServer
{
public:
    explicit FakeMediaServer(const char *conf_path);

    void run();
    static void stop();

private:
    bool create_worker(int serial);
    void stop_worker();

    static bool register_signal();
    static void signal_handler(int signal);

private:
    bool                worker_flag;
    const char         *conf_path;
    ServerConf         *server_conf;
    map<pid_t, int>     worker_map;

    static bool         interrupt;
};


#endif //FAKE_MEDIA_SERVER_H
