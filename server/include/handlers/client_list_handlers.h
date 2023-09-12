#ifndef CLIENT_LIST_HANDLERS_H
#define CLIENT_LIST_HANDLERS_H

#include <stdbool.h>
#include "client_list.h"

/* Set size, allocate memory, initialize mutex, set all file descriptors to -1 to signify no active clients */
void init_client_list(client_list_t* client_list, int size);

/* Insert client file descriptor into client list */
bool insert_client(client_list_t* client_list, int client_id);

/* Close and remove client file descriptor from client list */
int remove_client(client_list_t* client_list, int client_id);

/* Close and remove all file descriptors from client list */
void remove_all_clients(client_list_t* client_list);

/* Free file descrptors array and mutex, clear size */
void destroy_client_list(client_list_t* client_list);

#endif