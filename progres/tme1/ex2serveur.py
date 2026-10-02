from socket import *
import time
from threading import *

def handle_client(sock):
    try:
        while True:
            n = sock.recv(2048)
            if not n:
                break
            modifiedMessage = str(time.time()).encode('utf-8')
            sock.send(modifiedMessage)
            print('réponse envoyée')
    except (OSError):
        print("client est parti")
    finally: #quand le client n'envoie plus rien (ça ne lève pas d'erreur donc finally obligé)
        sock.close()

serverPort = 1234
serverSocket = socket(AF_INET,SOCK_STREAM)
serverSocket.bind(('',serverPort))
serverSocket.listen(47)
print('server ready')

while True:
    connectionSocket, address = serverSocket.accept()
    print('connection établie')
    Thread(target=handle_client,
        args=(connectionSocket,)).start()
