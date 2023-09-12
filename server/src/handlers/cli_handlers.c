#include "handlers/cli_handlers.h"

/* PLACEHOLDER */
void generic_print_response(pipe_io_t cli_io) {
    char res[BUFFER_SIZE];
    memset(res, 0, sizeof(res));

    read(cli_io.in, res, BUFFER_SIZE);
    puts(res);
}

void cli_help() {
    puts("help - list available commands");
    puts("list - list accepted connections");
    puts("attach {client_id} - enter chat with specified client");
    puts("disconnect {client_id} - remove specified client");
    puts("tail - listen to server logs");
    puts("exit - remove all connections and close session");
}

int cli_list_clients(pipe_io_t cli_io) {
    char cmd[] = "list";

    char res[BUFFER_SIZE];
    memset(res, 0, sizeof(res));

    write(cli_io.out, cmd, strlen(cmd)); /* Write the command to server */
    read(cli_io.in, res, BUFFER_SIZE); /* Read response from server */

    int client_list[MAX_CONNECTIONS];
    int size = deserialize_int_array(res, client_list);

    puts("(TESTING) Available clients:");
    for (int i = 0; i < size; i++) {
        if (client_list[i] != -1) {
            printf("client: %i\n", client_list[i]);
        }
    }

    return 1;
}

int cli_attach(pipe_io_t cli_io, int client_id) {
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s %i", "attach ", client_id);

    write(cli_io.out, buffer, strlen(buffer));

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        read(cli_io.in, buffer, BUFFER_SIZE);

        if (strcmp(buffer, "error") == 0) {
            puts("Error on server side");
            exit(EXIT_FAILURE);
        } else if (strcmp(buffer, "client") == 0) {
            write(cli_io.out, "recieved", strlen("recieved")); /* Acknowledge server - needs revision */
            memset(buffer, 0, sizeof(buffer));
            read(cli_io.in, buffer, BUFFER_SIZE);
            printf("Client %i: %s\n", client_id, buffer);

            /* Get user input */
            fputs("Enter message ('detach' to quit): ", stdout);
            memset(buffer, 0, sizeof(buffer));
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\r\n")] = 0; /* Remove trailing newline / return */
            write(cli_io.out, buffer, strlen(buffer));

            if (strcmp(buffer, "detach") == 0) {
                break;
            }
        } else {
            puts(buffer);
            if (strcmp(buffer, "client disconnected") == 0) {
                break;
            }
        }
    }

    return 1;
}

int cli_disconnect(pipe_io_t cli_io, int client_id) {
    char cmd[BUFFER_SIZE];
    memset(cmd, 0, sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "%s %i", "disconnect ", client_id);

    puts("cli_disconnect() not yet implemented");
    write(cli_io.out, cmd, strlen(cmd));

    generic_print_response(cli_io);

    return 1;
}

int cli_tail(pipe_io_t cli_io) {
    char cmd[] = "tail";

    char res[BUFFER_SIZE];
    memset(res, 0, sizeof(res));

    puts("cli_tail() not yet implemented");
    write(cli_io.out, cmd, strlen(cmd));

    generic_print_response(cli_io);

    return 1;
}

int cli_exit(pipe_io_t cli_io) {
    char cmd[] = "exit";

    char res[BUFFER_SIZE];
    memset(res, 0, sizeof(res));

    puts("cli_exit() not yet implemented");
    write(cli_io.out, cmd, strlen(cmd));

    generic_print_response(cli_io);

    return 1;
}