//
// Created by BoringWednesday on 2021/5/9.
//

#ifndef FAKE_MEDIA_SERVER_WORKER_H
#define FAKE_MEDIA_SERVER_WORKER_H

#include <sys/epoll.h>

class Worker {
public:
    Worker();
    void run();
    static void stop();

private:
    void event_loop();

    bool register_signal();
    static void signal_handler(int signal);

private:
    static bool     interrupt;
};


#endif //FAKE_MEDIA_SERVER_WORKER_H
