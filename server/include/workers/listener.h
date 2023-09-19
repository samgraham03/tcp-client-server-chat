#ifndef LISTENER_H
#define LISTENER_H

#include "client_list.h"

typedef struct {
    int exit;
    int server_socket_fd;
    client_list_t* client_list;
} listener_context_t;

/* Cleanup listener thread */
void listener_cleanup(void* arg);

/* listen for and accept new clients */
void* listener_worker(void* client);

#endif