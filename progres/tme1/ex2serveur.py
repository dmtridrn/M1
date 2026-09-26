from socket import *
import time
from threading import *

def handle_client(sock):
    try: #au cas ouuuuuu mais trql
        while True:
            n = sock.recv(2048)
            if not n:
                break
            modifiedMessage = str(time.time()).encode('utf-8')
            sock.send(modifiedMessage)
            print('réponse envoyée')
    except (ConnectionResetError, BrokenPipeError):
        print("client est parti")
    finally:
        sock.close()

serverPort = 1234
serverSocket = socket(AF_INET,SOCK_STREAM)
serverSocket.bind(('',serverPort))
serverSocket.listen(1)
print('server ready')

while True:
    connectionSocket, address = serverSocket.accept()
    print('connection établie')
    Thread(target=handle_client,
        args=(connectionSocket,)).start()
