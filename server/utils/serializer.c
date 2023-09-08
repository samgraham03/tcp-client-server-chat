#include "serializer.h"

int serialize_int_array(int arr[], int size, char* result) {
    if (size == 0) {
        result[0] = '\0';
        return 0;
    }

    char temp[20];

    sprintf(result, "%d", arr[0]);
    for (int i = 1; i < size; i++) {
        sprintf(temp, ",%d", arr[i]);
        strcat(result, temp);
    }

    return 1;
}

int deserialize_int_array(char* input, int* arr) {
    int size = 0;

    char* token = strtok(input, ",");
    while (token != NULL) {
        arr[size] = atoi(token);
        token = strtok(NULL, ",");
        size++;
    }

    return size;
}