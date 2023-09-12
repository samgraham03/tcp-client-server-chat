#ifndef SERVER_HANDLERS_H
#define SERVER_HANDLERS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>

#include "server_config.h"
#include "pipe_comm.h"

#include "utils/serializer.h"

/* List accepted connections */
int server_list_clients(pipe_io_t server_io);

/* Enter chat with specified client */
int server_attach(pipe_io_t server_io, int* client_socket_fd);

/* Remove specified client */
int server_disconnect(pipe_io_t server_io, int client_socket_fd);

/* Listen to server logs */
int server_tail(pipe_io_t server_io);

/* Remove all connections and close session */
int server_exit(pipe_io_t server_io);

#endif