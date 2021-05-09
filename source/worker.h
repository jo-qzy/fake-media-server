//
// Created by BoringWednesday on 2021/5/9.
//

#ifndef FAKE_MEDIA_SERVER_WORKER_H
#define FAKE_MEDIA_SERVER_WORKER_H


class Worker {
    // TODO: singleton mode
public:
    void Run();
    static void Stop();
private:
    static bool RegisterSignal();
    static void SignalHandler(int signal);

private:

    static bool     interrupt;
};


#endif //FAKE_MEDIA_SERVER_WORKER_H
