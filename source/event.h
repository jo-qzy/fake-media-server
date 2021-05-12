//
// Created by BoringWednesday on 2021/5/8.
//

#ifndef FAKE_MEDIA_SERVER_EVENT_H
#define FAKE_MEDIA_SERVER_EVENT_H


#include <util/buffer.h>

class BaseEvent
{
public:
    virtual int read() = 0;
    virtual int write() = 0;

    int event_in();
    int event_out();
    int event_in_out();
private:
    int fd;


};


#endif //FAKE_MEDIA_SERVER_EVENT_H
