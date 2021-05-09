//
// Created by BoringWednesday on 2021/5/9.
//

#ifndef FAKE_MEDIA_SERVER_H
#define FAKE_MEDIA_SERVER_H

#include <map>
#include <string>

using namespace std;

class FakeMediaServer
{
public:
    explicit FakeMediaServer(const char *conf_path);

    void Run();
    static void Stop();

private:
    bool CreateWorker(int serial);
    void StopAllWorker();

    static bool RegisterSignal();
    static void SignalHandler(int signal);

private:
    bool                master_mode;
    bool                worker_flag;
    const char         *conf_path;
    map<pid_t, int>     worker_map;

    static bool         interrupt;
};


#endif //FAKE_MEDIA_SERVER_H
