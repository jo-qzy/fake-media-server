//
// Created by BoringWednesday on 2021/5/19.
//

#ifndef FAKE_MEDIA_CLIENT_ECHO_CLIENT_H
#define FAKE_MEDIA_CLIENT_ECHO_CLIENT_H


#include "echo_internal.h"

#if defined(__cplusplus)
extern "C" {
#endif

struct echo_client_t;

struct echo_client_handler_t
{
    int (*send)(void *param, const void *packet, size_t bytes);
    int (*onpacket)(void *param, const void *data, size_t bytes, uint32_t sequence);
};

struct echo_client_t * echo_client_create(void *param, struct echo_client_handler_t *handler);

int echo_client_destroy(struct echo_client_t *echo);

int echo_client_input(struct echo_client_t *echo, const void *data, size_t bytes);

int echo_client_send_message(struct echo_client_t *echo, const void* data, size_t bytes);

#if defined(__cplusplus)
}
#endif


#endif //FAKE_MEDIA_CLIENT_ECHO_CLIENT_H
