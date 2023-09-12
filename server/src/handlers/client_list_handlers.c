#include "handlers/client_list_handlers.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void init_client_list(int client_list[], int size) {
    for (int i = 0; i < size; i++) {
        client_list[i] = -1;
    }
}

bool insert_client(int client_list[], int size, int client_id) {
    int free_index = -1;
    for (int i = 0; i < size; i++) {
        if (client_list[i] == client_id) { /* Client already exists in list */
            free_index = -1;
            break;
        } else if (free_index == -1 && client_list[i] == -1) {
            free_index = i;
        }
    }
    if (free_index != -1) {
        client_list[free_index] = client_id;
        return true;
    }
    return false;
}

int remove_client(int client_list[], int size, int client_id) {
    bool found = false;
    for (int i = 0; i < size; i++) {
        if (client_list[i] == client_id) {
            if (!found) {
                found = true;
                close(client_id);
            }
            client_list[i] = -1;
        }
    }
    return found;
}

void remove_all_clients(int client_list[], int size) {
    bool closed = false;
    for (int i = 0; i < size; i++) {
        if (client_list[i] != -1) {
            close(client_list[i]);
            client_list[i] = -1;
        }
    }
}