# tcp-client-server-chat

Compile server
```
gcc -o server.out server.c ./utils/serializer.c cli_handlers.c server_handlers.c

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