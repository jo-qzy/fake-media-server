//
// Created by BoringWednesday on 2021/5/16.
//

#ifndef FAKE_MEDIA_SERVER_ADAPTER_H
#define FAKE_MEDIA_SERVER_ADAPTER_H

class Event;
class IOBuffer;

class Adapter
{
public:
    explicit Adapter(Event *ev);
    virtual ~Adapter() = default;

    virtual int connect() = 0;
    virtual int accept(Event *ev) = 0;
    virtual int read_handler(IOBuffer *buffer) = 0;
    //virtual int write_handler(IOBuffer *buffer) = 0;

protected:
    Event      *event;

};


#endif //FAKE_MEDIA_SERVER_ADAPTER_H
