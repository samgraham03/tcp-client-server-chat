#ifndef CLIENT_LIST_H
#define CLIENT_LIST_H

#include "server_config.h"
#include <pthread.h>

typedef struct {
    int size;
    int* fds;
    pthread_mutex_t lock;
} client_list_t;

#endif