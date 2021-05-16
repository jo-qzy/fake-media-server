//
// Created by BoringWednesday on 2021/5/16.
//

#ifndef FAKE_MEDIA_SERVER_PROTOCOL_PLUGIN_H
#define FAKE_MEDIA_SERVER_PROTOCOL_PLUGIN_H

class Event;

class Adapter
{
public:
    explicit Adapter(Event *ev);

    virtual int connect();
    virtual int accept(Event *ev);
    virtual int read_handler() = 0;
    virtual int write_handler() = 0;

protected:
    Event      *event;

};


#endif //FAKE_MEDIA_SERVER_PROTOCOLPLUG_H
