from socket import *
from select import *
from pathlib import Path

#setup la socket
serverPort = 1234
serverSocket = socket(AF_INET,SOCK_DGRAM)
serverSocket.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1) #pour address already in use 
serverSocket.bind(('',serverPort))
serverSocket.listen(47)
serverSocket.setblocking(False) #si recv bloque ca paralyse pas tout

