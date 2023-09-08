# tcp-client-server-chat

Compile server
```
gcc -o server.out ./server/server.c ./server/utils/serializer.c ./server/cli_handlers.c ./server/server_handlers.c
```
Run server
```
./server.out
```
Run Java client
```
java ./clients/JavaClient.java
```
Run Python client
```
python3 ./clients/python_client.py
```