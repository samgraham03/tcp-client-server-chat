#ifndef CLI_HANDLERS_H
#define CLI_HANDLERS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "server_config.h"
#include "pipe_comm.h"

#include "utils/serializer.h"

/* List available commands */
void cli_help();

/* List accepted connections */
int cli_list_clients(pipe_io_t cli_io);

/* Enter chat with specified client */
int cli_attach(pipe_io_t cli_io, int client_id);

/* Remove specified client */
int cli_disconnect(pipe_io_t cli_io, int client_id);

/* Listen to server logs */
int cli_tail(pipe_io_t cli_io);

/* Remove all connections and close session */
int cli_exit(pipe_io_t cli_io);

#endif