#include "workers/listener.h"

#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <signal.h>

#include "handlers/client_list_handlers.h"

void listener_cleanup(void* arg) {
    puts("In thread cleanup");
    listener_context_t* listener_context = (listener_context_t*)arg;
    pthread_mutex_unlock(&(listener_context->client_list->lock));
}

void* listener_worker(void* arg) {
    /* Ignore SIGPIPE errors */
    signal(SIGPIPE, SIG_IGN); /* TODO: Review */

    listener_context_t* listener_context = (listener_context_t*)arg;
    pthread_cleanup_push(listener_cleanup, (void*)listener_context);

    while (1) {
        struct timeval timeout;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;

        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(listener_context->server_socket_fd, &readfds);

        int is_full = 1; /* TODO: Rview */

        int max_fd = listener_context->server_socket_fd;

        /* TODO: Review need for client list (fd_set) */
        pthread_mutex_lock(&(listener_context->client_list->lock));
        for (int i = 0; i < listener_context->client_list->size; i++) {
            int client_fd = listener_context->client_list->fds[i];
            if (client_fd != -1) {
                FD_SET(client_fd, &readfds);
            }
            if (client_fd > max_fd) {
                max_fd = client_fd;
            }
            if (client_fd == -1) {
                is_full = 0; /* TODO: Review */
            }
        }
        pthread_mutex_unlock(&(listener_context->client_list->lock));

        int select_result = select(max_fd + 1, &readfds, NULL, NULL, &timeout);

        if (select_result == -1) {
            perror("select - listener worker failed");
            break;
        } else if (select_result == 0 || listener_context->exit) { /* Select timed out */
            if (listener_context->exit) {
                puts("Listener worker exiting");
                break;
            }
        } else {
            if (!is_full) {
                int client_socket_fd = accept(listener_context->server_socket_fd, NULL, NULL);
                if (client_socket_fd != -1) {
                    insert_client(listener_context->client_list, client_socket_fd);
                }
            }
        }
    }

    pthread_cleanup_pop(1);
    pthread_exit(NULL);
}