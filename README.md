# SocketServer

C++ Server socket, allows multiple connections and manage it throug threads
You can send message to another client or a all clients.

example message from client to client:
```
{"action": "MSG","destination": {"ip": "127.0.0.1","port": 3301,"socket": 4},"msg": "Hola Soy cliente 3","source": {"ip": "127.0.0.1","port": 44520,"socket": 6}}
```

ecample message from client to broadcast
```
{"action": "MSG","destination": {"ip": "127.0.0.1","port": 3301,"socket": 2147483647},"msg": "Hola Soy cliente 3","source": {"ip": "127.0.0.1","port": 44520,"socket": 6}}
```
