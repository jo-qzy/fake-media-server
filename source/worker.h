//
// Created by BoringWednesday on 2021/5/9.
//

#ifndef FAKE_MEDIA_SERVER_WORKER_H
#define FAKE_MEDIA_SERVER_WORKER_H


class Worker {
    // TODO: singleton mode
public:
    void run();
    static void stop();
private:
    bool register_signal();
    static void signal_handler(int signal);

private:

    static bool     interrupt;
};


#endif //FAKE_MEDIA_SERVER_WORKER_H
