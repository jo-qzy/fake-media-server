//
// Created by BoringWednesday on 2021/5/8.
//

#ifndef FAKE_MEDIA_SERVER_EVENT_H
#define FAKE_MEDIA_SERVER_EVENT_H


class BaseEvent
{
public:
    virtual int read() = 0;
    virtual int write() = 0;

};


#endif //FAKE_MEDIA_SERVER_EVENT_H
