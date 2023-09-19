CC = gcc
CFLAGS = -Wall -g -I ./include/
LIBS = -pthread

SRC_DIR = ./src
HANDLERS_DIR = $(SRC_DIR)/handlers
WORKERS_DIR = $(SRC_DIR)/workers
UTILS_DIR = $(SRC_DIR)/utils

SOURCES = \
	./server.c \
	$(HANDLERS_DIR)/cli_handlers.c \
	$(HANDLERS_DIR)/client_list_handlers.c \
	$(HANDLERS_DIR)/server_handlers.c \
	$(WORKERS_DIR)/listener.c \
	$(UTILS_DIR)/serializer.c

server.out:
	$(CC) $(CFLAGS) -o server.out $(SOURCES) $(LIBS)

clean:
	rm -f server.out