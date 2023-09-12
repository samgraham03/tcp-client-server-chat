#ifndef PIPE_COMM_H
#define PIPE_COMM_H

#define BUFFER_SIZE 1024

#define READ_END 0
#define WRITE_END 1

typedef struct {
    int in;
    int out;
} pipe_io_t;

#endif