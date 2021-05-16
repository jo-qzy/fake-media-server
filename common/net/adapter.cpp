//
// Created by BoringWednesday on 2021/5/16.
//

#include "adapter.h"

Adapter::Adapter(Event *ev)
    : event(ev)
{}

int Adapter::connect()
{
    return 0;
}

int Adapter::accept(Event *ev)
{
    return 0;
}
