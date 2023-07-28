#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void help(int context);
static void list_clients();
static int attach(int client_id);
static int disconnect(int context, int client_id);
static int detach(int client_id);
static void cleanup(int context);

#define CLI_CONTEXT 0
#define CLIENT_CONTEXT 1

static void help(int context) {
    puts("help - list available commands");
    if (context == CLI_CONTEXT) {
        puts("switch - switch to CLIENT_CONTEXT (FOR TESTING ONLY)");
        puts("list - list accepted connections");
        puts("attach {client_id} - enter chat with specified client");
        puts("disconnect {client_id} - remove specified client");
    } else if (context == CLIENT_CONTEXT) {
        puts("switch - switch to CLI_CONTEXT (FOR TESTING ONLY)");
        puts("detach - exit chat with client");
        puts("disconnect - remove client");
    } else {
        puts("Error: undetermined context");
        help(CLI_CONTEXT);
        help(CLIENT_CONTEXT);
    }
    puts("exit - remove all connections and close session");
}

static void list_clients() {
    puts("list_clients()");
}

static int attach(int client_id) {
    printf("attach(%i)\n", client_id);
    return 1;
}

static int disconnect(int context, int client_id) {
    printf("disconnect(%i, %i)\n", context, client_id);
    return 1;
}

static int detach(int client_id) {
    printf("detach(%i)\n", client_id);
    return 1;
}

static void cleanup(int context) {
    printf("cleanup(%i)\n", context);
}

int main() {
    int context = CLI_CONTEXT;
    
    help(context);

    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    char cmd[100];
    memset(cmd, 0, sizeof(cmd));

    int client_id = 0;

    while (1) {
        fputs("$ ", stdout);
        fgets(buffer, sizeof(buffer), stdin);

        int inputs;

        if (context == CLI_CONTEXT) {
            inputs = sscanf(buffer, "%s %i", cmd, &client_id);
        } else if (context == CLIENT_CONTEXT) {
            inputs = sscanf(buffer, "%s", cmd);
        } else {
            puts("Error: context not recognized");
            exit(EXIT_FAILURE);
        }

        if (inputs != 1 && inputs != 2) {
            puts("\nERROR: Command not recognized - try \"help\" to see options");
        } else if (strcmp(cmd, "help") == 0) {
            help(context);
        } else if (strcmp(cmd, "switch") == 0) {
            context = (context == CLI_CONTEXT) ? CLIENT_CONTEXT : CLI_CONTEXT; // FOR TESTING ONLY
        } else if (strcmp(cmd, "exit") == 0) {
            cleanup(context);
            exit(EXIT_SUCCESS);
        } else if (context == CLI_CONTEXT) {
            if (strcmp(cmd, "list") == 0) {
                list_clients();
            } else if (strcmp(cmd, "attach") == 0) {
                if (inputs == 2) {
                    attach(client_id);
                } else {
                    list_clients();
                    puts("try: attach {client_id}");
                }
            } else if (strcmp(cmd, "disconnect") == 0) {
                if (inputs == 2) {
                    disconnect(context, client_id);
                } else {
                    list_clients();
                    puts("try: disconnect {client_id}");
                }
            } else {
                puts("\nCommand not recognized - try \"help\" to see options");
            }
        } else if (context == CLIENT_CONTEXT) {
            if (strcmp(cmd, "detach") == 0) {
                detach(client_id);
            } else if (strcmp(cmd, "disconnect") == 0) {
                disconnect(context, client_id);
            } else {
                puts("\nCommand not recognized - try \"help\" to see options");
            }
        }

        memset(buffer, 0, sizeof(buffer));
        memset(cmd, 0, sizeof(cmd));
    }
}