from socket import *
import time
from threading import *

def handle_client(sock):
    while True:
        sock.recv(2048)
        modifiedMessage = str(time.time()).encode('utf-8')
        sock.send(modifiedMessage)
        print('réponse envoyée')

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
