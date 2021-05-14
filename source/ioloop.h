//
// Created by BoringWednesday on 2021/5/13.
//

#ifndef FAKE_MEDIA_SERVER_IOLOOP_H
#define FAKE_MEDIA_SERVER_IOLOOP_H


class IOLoop
{
public:
    IOLoop(int max_connections);
    ~IOLoop();

    void run();
    void stop();

private:
    void event_loop();

private:
    int             poll_id;
    void           *event_ptr;
    int             event_num;
    bool            interrupt;
};


#endif //FAKE_MEDIA_SERVER_IOLOOP_H
