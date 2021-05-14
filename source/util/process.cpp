//
// Created by BoringWednesday on 2021/5/10.
//

#include "process.h"
#include <signal.h>

int register_signal(int sig, void (*handler)(int))
{
    if (signal(sig, handler) == SIG_ERR) {
        return -1;
    }

    return 0;
}
