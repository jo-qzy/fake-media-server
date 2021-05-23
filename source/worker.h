//
// Created by BoringWednesday on 2021/5/9.
//

#ifndef FAKE_MEDIA_SERVER_WORKER_H
#define FAKE_MEDIA_SERVER_WORKER_H

#include <net/io_loop.h>

class Worker : public IOLoop
{
public:
    static Worker * create_worker(int max_events);
    static Worker * get_worker();

private:
    explicit Worker(int max_events);
    virtual ~Worker();

    virtual int initial_event() override;

    static void signal_handler(int signal);

private:
    static Worker *worker_instance;
};


#endif //FAKE_MEDIA_SERVER_WORKER_H
