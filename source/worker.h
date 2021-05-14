//
// Created by BoringWednesday on 2021/5/9.
//

#ifndef FAKE_MEDIA_SERVER_WORKER_H
#define FAKE_MEDIA_SERVER_WORKER_H

#include "ioloop.h"

class Worker
{
public:
    Worker();
    ~Worker();

    void run();

private:
    static void signal_handler(int signal);

private:
    IOLoop *io_loop;
};


#endif //FAKE_MEDIA_SERVER_WORKER_H
