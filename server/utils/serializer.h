#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Serialize an array of integers to a string */
int serialize_int_array(int arr[], int size, char* result);

/* Deserialize a string to an array of integers */
int deserialize_int_array(char* input, int* arr);

#endif