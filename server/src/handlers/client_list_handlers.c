#include "handlers/client_list_handlers.h"
#include <stdbool.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void init_client_list(client_list_t* client_list, int size) {
    client_list->size = size;

    client_list->fds = calloc(size, sizeof(int));
    if (client_list->fds == NULL) {
        perror("fds allocation failed");
        exit(EXIT_FAILURE);
    }

    if (pthread_mutex_init(&client_list->lock, NULL) != 0) {
        perror("mutex init failed");
        exit(EXIT_FAILURE);
    }

    pthread_mutex_lock(&client_list->lock);
    for (int i = 0; i < client_list->size; i++) {
        client_list->fds[i] = -1;
    }
    pthread_mutex_unlock(&client_list->lock);
}

bool insert_client(client_list_t* client_list, int client_id) {
    pthread_mutex_lock(&client_list->lock);
    int free_index = -1;
    for (int i = 0; i < client_list->size; i++) {
        if (client_list->fds[i] == client_id) { /* Client already exists in list */
            free_index = -1;
            break;
        } else if (free_index == -1 && client_list->fds[i] == -1) {
            free_index = i;
        }
    }
    if (free_index != -1) {
        client_list->fds[free_index] = client_id;
        return true;
    }
    return false;
    pthread_mutex_unlock(&client_list->lock);
}

int remove_client(client_list_t* client_list, int client_id) {
    pthread_mutex_lock(&client_list->lock);
    bool found = false;
    for (int i = 0; i < client_list->size; i++) {
        if (client_list->fds[i] == client_id) {
            if (!found) {
                found = true;
                close(client_id);
            }
            client_list->fds[i] = -1;
        }
    }
    return found;
    pthread_mutex_unlock(&client_list->lock);
}

void remove_all_clients(client_list_t* client_list) {
    pthread_mutex_lock(&client_list->lock);
    bool closed = false;
    for (int i = 0; i < client_list->size; i++) {
        if (client_list->fds[i] != -1) {
            close(client_list->fds[i]);
            client_list->fds[i] = -1;
        }
    }
    pthread_mutex_unlock(&client_list->lock);
}

void destroy_client_list(client_list_t* client_list) {
    pthread_mutex_lock(&client_list->lock);
    client_list->size = 0;
    if (client_list->fds != NULL) {
        free(client_list->fds);
        client_list->fds = NULL;
    }
    pthread_mutex_unlock(&client_list->lock);
    pthread_mutex_destroy(&client_list->lock);
}