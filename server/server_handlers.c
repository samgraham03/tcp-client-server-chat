#include "server_handlers.h"

int server_list_clients(pipe_io_t server_io) {
    int test_client_list[] = {1, 2, 3, 4, 5};
    int size = sizeof(test_client_list) / sizeof(int);

    char msg[20*MAX_CONNECTIONS];
    serialize_int_array(test_client_list, size, msg);

    write(server_io.out, msg, strlen(msg));

    return 1;
}

int server_attach(pipe_io_t server_io, int* client_socket_fd) {
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));

    while (1) {
        /* Recieve message from client */
        memset(buffer, 0, sizeof(buffer));
        ssize_t recv_size = recv(*client_socket_fd, buffer, sizeof(buffer) - 1, 0);
        if (recv_size == 0) {
            write(server_io.out, "client disconnected", strlen("client disconnected"));
            if (*client_socket_fd != -1) {
                puts("closing client socket");
                close(*client_socket_fd);
                *client_socket_fd = -1;
            }
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
        send(*client_socket_fd, buffer, strlen(buffer), 0);
    }

    return 1;
}

int server_disconnect(pipe_io_t server_io, int client_socket_fd) {
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