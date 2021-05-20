//
// Created by BoringWednesday on 2021/5/19.
//

#ifndef FAKE_MEDIA_SERVER_ECHO_SERVER_H
#define FAKE_MEDIA_SERVER_ECHO_SERVER_H


#include "echo_internal.h"

#if defined(__cplusplus)
extern "C" {
#endif

struct echo_server_t;

struct echo_server_handler_t
{
    int (*send)(void *param, const void *packet, size_t bytes);
};

struct echo_server_t * echo_server_create(void *param, struct echo_server_handler_t *handler);

int echo_server_destroy(struct echo_server_t *echo);

int echo_server_input(struct echo_server_t *echo, const void *data, size_t bytes);

#if defined(__cplusplus)
}
#endif


#endif //FAKE_MEDIA_SERVER_ECHO_SERVER_H
