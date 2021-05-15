//
// Created by BoringWednesday on 2021/5/13.
//

#ifndef FAKE_MEDIA_SERVER_IOLOOP_H
#define FAKE_MEDIA_SERVER_IOLOOP_H

class Event;

class IOLoop
{
public:
    explicit IOLoop(int max_events);
    virtual ~IOLoop();

    int create();
    void run();
    static void stop();

    int add_event(Event *ev);
    int del_event(Event *ev);
    int mod_event(Event *ev);

protected:
    void event_loop();

protected:
    int             poll_id;
    void           *event_ptr;
    int             event_num;

    static bool     interrupt;
};


#endif //FAKE_MEDIA_SERVER_IOLOOP_H