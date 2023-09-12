#ifndef CLIENT_LIST_HANDLERS_H
#define CLIENT_LIST_HANDLERS_H

#include <stdbool.h>

/* Set all elements to -1 to signify no active clients */
void init_client_list(int client_list[], int size);

/* Insert client file descriptor into client list */
bool insert_client(int client_list[], int size, int client_id);

/* Close and remove client file descriptor from client list */
int remove_client(int client_list[], int size, int client_id);

/* Close and remove all file descriptors from client list */
void remove_all_clients(int client_list[], int size);

#endif