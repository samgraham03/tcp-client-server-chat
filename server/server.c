#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <signal.h>

#define PORT 8080
#define REUSE_ADDR

#define BUFFER_SIZE 1024

int server_socket_fd = -1;
int client_socket_fd = -1;

void cleanup() {
    puts("");
    if (client_socket_fd != -1) {
        puts("closing client socket");
        close(client_socket_fd);
        client_socket_fd = -1;
    }
    if (server_socket_fd != -1) {
        puts("closing server socket");
        close(server_socket_fd);
        server_socket_fd = -1;
    }
}

void sigint_handler(int signum) {
    cleanup();
    exit(EXIT_SUCCESS);
}

int main(int argc, char** argv) {
    /* Register SIGINT signal handler */
    __sighandler_t signal_result = signal(SIGINT, sigint_handler);
    if (signal_result == SIG_ERR) {
        perror("Error: signal()");
        exit(EXIT_FAILURE);
    }

    /* Configure server address info */
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET; /* IPV4 */
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    /* Create server socket */
    server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_fd == -1) {
        perror("Error: socket()\n");
        exit(EXIT_FAILURE);
    }

#ifdef REUSE_ADDR
    /* Bypass socket TIME_WAIT status when restarting server */
    int server_socket_opt_result = setsockopt(server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));
    if (server_socket_opt_result == -1) {
        perror("Error: setsockopt()");
        cleanup();
        exit(EXIT_FAILURE);
    }
#endif

    /* Bind server socket to address info */
    int bind_result = bind(server_socket_fd, (struct sockaddr*) &server_addr, sizeof(server_addr));
    if (bind_result == -1) {
        perror("Error: bind()\n");
        cleanup();
        exit(EXIT_FAILURE);
    }

    /* Listen for incoming connections */
    int listen_result = listen(server_socket_fd, 1);
    if (listen_result == -1) {
        perror("Error: listen()\n");
        cleanup();
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));

    while (1) {
        printf("Listening for connections on port %i...\n", ntohs(server_addr.sin_port));

        /* Await a connection */
        client_socket_fd = accept(server_socket_fd, NULL, NULL);
        if (client_socket_fd == -1) {
            perror("Error: accept()");
            cleanup();
            exit(EXIT_FAILURE);
        } else {
            puts("Client accepted");
        }

        while (1) {
            /* Recieve message from client */
            memset(buffer, 0, sizeof(buffer));
            ssize_t recv_size = recv(client_socket_fd, buffer, sizeof(buffer) - 1, 0);
            if (recv_size == -1) {
                perror("Error: recv()");
                cleanup();
                exit(EXIT_FAILURE);
            } else if (recv_size == 0) {
                puts("Client disconnected");
                if (client_socket_fd != -1) {
                    puts("closing client socket");
                    close(client_socket_fd);
                    client_socket_fd = -1;
                }
                break;
            }
            buffer[strcspn(buffer, "\r\n")] = 0; // Remove trailing newline / return
            printf("Client: %s\n", buffer);

            /* Get user input */
            fputs("Enter message ('exit' to quit): ", stdout);
            memset(buffer, 0, sizeof(buffer));
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\r\n")] = 0; // Remove trailing newline / return

            if (strcmp(buffer, "exit") == 0) {
                cleanup();
                exit(EXIT_SUCCESS);
            }

            /* Send message to client */
            ssize_t send_size = send(client_socket_fd, buffer, strlen(buffer), 0);
            if (send_size == -1) {
                perror("Error: send()");
                cleanup();
                exit(EXIT_FAILURE);
            }
        }
    }

    puts("Closing session");
    cleanup();

    return 0;
}