//
// Created by BoringWednesday on 2021/5/8.
//

#ifndef FAKE_MEDIA_SERVER_EVENT_H
#define FAKE_MEDIA_SERVER_EVENT_H

class IOLoop;

class Event
{
public:
    explicit Event(IOLoop *loop, int fd = -1);
    virtual ~Event();

    int get_fd();
    int get_events();

    virtual int on_read() = 0;
    virtual int on_write() = 0;

    void enable_read();
    void enable_write();
    void disable_read();
    void disable_write();

protected:
    void add_event();
    void del_event();

protected:
    int         fd;
    int         events;
    bool        is_add;

    IOLoop     *io_loop;

};


#endif //FAKE_MEDIA_SERVER_EVENT_H
