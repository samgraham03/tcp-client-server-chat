#include "handlers/server_handlers.h"

#include "handlers/client_list_handlers.h"

int server_list_clients(pipe_io_t server_io, client_list_t* list) {
    char serialized[20*MAX_CONNECTIONS];
    serialize_int_array(list->fds, list->size, serialized);
    write(server_io.out, serialized, strlen(serialized));
    return 1;
}

int server_attach(pipe_io_t server_io, client_list_t* list, int client_socket_fd) {
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));

    while (1) {
        /* Recieve message from client */
        memset(buffer, 0, sizeof(buffer));
        ssize_t recv_size = recv(client_socket_fd, buffer, sizeof(buffer) - 1, 0);
        if (recv_size == 0) {
            write(server_io.out, "client disconnected", strlen("client disconnected"));
            remove_client(list, client_socket_fd);
            break;
        }
        buffer[strcspn(buffer, "\r\n")] = 0; /* Remove trailing newline / return */

        /* Send client response to CLI */
        write(server_io.out, "client", strlen("client"));
        read(server_io.in, &(char[BUFFER_SIZE]){}, BUFFER_SIZE); /* Signal from CLI to forward response - needs revision */
        write(server_io.out, buffer, strlen(buffer));

        /* Get server response from CLI */
        memset(buffer, 0, sizeof(buffer));
        read(server_io.in, buffer, BUFFER_SIZE);

        if (strcmp(buffer, "detach") == 0) {
            break;
        }

        /* Send message to client */
        send(client_socket_fd, buffer, strlen(buffer), 0);
    }

    return 1;
}

int server_disconnect(pipe_io_t server_io, client_list_t* list, int client_socket_fd) {
    char msg[] = "server_disconnect() not yet implemented";
    write(server_io.out, msg, strlen(msg));
    return 1;
}

int server_tail(pipe_io_t server_io) {
    char msg[] = "server_tail() not yet implemented";
    write(server_io.out, msg, strlen(msg));
    return 1;
}

int server_exit(pipe_io_t server_io) {
    char msg[] = "server_exit() not yet implemented";
    write(server_io.out, msg, strlen(msg));
    return 1;
}