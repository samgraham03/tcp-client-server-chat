#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>
#include <pthread.h>

#include "pipe_comm.h"
#include "client_list.h"

#include "handlers/server_handlers.h"
#include "handlers/cli_handlers.h"
#include "handlers/client_list_handlers.h"

#include "workers/listener.h"

int main(int argc, char** argv) {
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));

    char cmd[100];
    memset(cmd, 0, sizeof(cmd));

    int server_to_cli[2];
    if (pipe(server_to_cli) == -1) {
        perror("Server to CLI pipe failed");
        exit(EXIT_FAILURE);
    }

    int cli_to_server[2];
    if (pipe(cli_to_server) == -1) {
        perror("CLI to server pipe failed");
        exit(EXIT_FAILURE);
    }

    pipe_io_t server_io;
    server_io.in = cli_to_server[READ_END];
    server_io.out = server_to_cli[WRITE_END];

    pipe_io_t cli_io;
    cli_io.in = server_to_cli[READ_END];
    cli_io.out = cli_to_server[WRITE_END];

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    /* Server - parent process */
    else if (pid > 0) {
        puts("SERVER STARTED");

        close(cli_io.in);
        close(cli_io.out);

        /* Configure server address info */
        struct sockaddr_in server_addr;
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET; /* IPV4 */
        server_addr.sin_port = htons(PORT);
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

        /* Create server socket */
        int server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_socket_fd == -1) {
            perror("Error: socket()\n");
            exit(EXIT_FAILURE);
        }

        client_list_t client_list;
        init_client_list(&client_list, MAX_CONNECTIONS);

#ifdef REUSE_ADDR
        /* Bypass socket TIME_WAIT status when restarting server */
        int server_socket_opt_result = setsockopt(server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));
        if (server_socket_opt_result == -1) {
            perror("Error: setsockopt()");
            close(server_socket_fd);
            exit(EXIT_FAILURE);
        }
#endif

        /* Bind server socket to address info */
        int bind_result = bind(server_socket_fd, (struct sockaddr*) &server_addr, sizeof(server_addr));
        if (bind_result == -1) {
            perror("Error: bind()\n");
            close(server_socket_fd);
            exit(EXIT_FAILURE);
        }

        /* Listen for incoming connections */
        int listen_result = listen(server_socket_fd, MAX_CONNECTIONS);
        if (listen_result == -1) {
            perror("Error: listen()\n");
            close(server_socket_fd);
            exit(EXIT_FAILURE);
        }

        listener_context_t listener_context;
        listener_context.exit = 0;
        listener_context.server_socket_fd = server_socket_fd;
        listener_context.client_list = &client_list;

        /* --register sigint handler to cleanup and set exit to 0-- */

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, listener_worker, (void*)&listener_context);

        while (1) {
            int client_id = -1;

            read(server_io.in, buffer, BUFFER_SIZE);
            buffer[BUFFER_SIZE - 1] = '\0';

            int inputs = sscanf(buffer, "%s %i", cmd, &client_id);

            if (inputs == 1 && strcmp(cmd, "list") == 0) {
                server_list_clients(server_io, &client_list);
            } else if (inputs == 2 && strcmp(cmd, "attach") == 0) {
                server_attach(server_io, &client_list, client_id);
            } else if (inputs == 2 && strcmp(cmd, "disconnect") == 0) {
                server_disconnect(server_io, &client_list, client_id);
            } else if (inputs == 1 && strcmp(cmd, "tail") == 0) {
                server_tail(server_io);
            } else if (inputs == 1 && strcmp(cmd, "exit") == 0) {
                server_exit(server_io);
                break;
            } else {
                write(server_io.out, "error", strlen("error"));
            }

            memset(buffer, 0, sizeof(buffer));
            memset(cmd, 0, sizeof(cmd));
        }

        listener_context.exit = 1;
        pthread_join(thread_id, NULL);
        destroy_client_list(&client_list);

        close(server_io.in);
        close(server_io.out);
    }

    /* CLI - child process */
    else {
        sleep(1);
        puts("CLI STARTED");

        close(server_io.in);
        close(server_io.out);

        cli_help();

        while (1) {
            int client_id = -1;

            fputs("$ ", stdout);
            fgets(buffer, sizeof(buffer), stdin);

            int inputs = sscanf(buffer, "%s %i", cmd, &client_id);

            if (inputs == 1 && strcmp(cmd, "help") == 0) {
                cli_help();
            } else if (inputs == 1 && strcmp(cmd, "list") == 0) {
                cli_list_clients(cli_io);
            } else if (inputs == 2 && strcmp(cmd, "attach") == 0) {
                cli_attach(cli_io, client_id);
                cli_attach(cli_io, -1);
            } else if (inputs == 1 && strcmp(cmd, "attach") == 0) {
                cli_list_clients(cli_io);
                puts("try: attach {client_id}");
            } else if (inputs == 2 && strcmp(cmd, "disconnect") == 0) {
                cli_disconnect(cli_io, client_id);
            } else if (inputs == 1 && strcmp(cmd, "disconnect") == 0) {
                cli_list_clients(cli_io);
                puts("try: disconnect {client_id}");
            } else if (inputs == 1 && strcmp(cmd, "tail") == 0) {
                cli_tail(cli_io);
            } else if (inputs == 1 && strcmp(cmd, "exit") == 0) {
                cli_exit(cli_io);
                break;
            } else {
                perror("Command not recognized - try \"help\" to see options");
            }

            memset(buffer, 0, sizeof(buffer));
            memset(cmd, 0, sizeof(cmd));
        }

        close(cli_io.in);
        close(cli_io.out);
    }

    exit(EXIT_SUCCESS);
}