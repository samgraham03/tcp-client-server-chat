# tcp-client-server-chat

Compile server
```
make -C ./server
```
Run server
```
./server/server.out
```
Run Java client
```
java ./clients/JavaClient.java
```
Run Python client
```
python3 ./clients/python_client.py
```

## Project

### Goal

This project aims to implement a TCP server in C for client-server type communication.
I began working on this project after taking interest in systems programing and concurrency.
Currently this project is very much a WIP and a learning experience for me.
Once I squash the remaining bugs, my hope is that this project can serve as a useful reference for anyone looking to get started with sockets or multi-threading.

### Status

All core features have been officially implemented.
- Running the program starts the CLI and Server on seperate processes (fork)
- The server creates a host socket and listens for incomming connections
- Data pipes allow the Server and CLI to communicate with each other
- The server starts a worker thread that loops on accepting new connections
- A client list supports the recording of simultanious connections
- The CLI allows the user to choose which client to engage

## Todo

- Add server sigint handler (cleanup)
- Add "attach" sigint handler (replace buggy "detach")
- Review client list (fd_set might be able to replace it entirely)
- Re-develop pipe communication agreement standard (serialize structures with type and data fields)
- Carefully review error handling / lack thereof