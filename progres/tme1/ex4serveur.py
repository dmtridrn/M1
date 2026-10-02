from socket import *

serverPort = 1234
serverSocket = socket(AF_INET,SOCK_DGRAM)
serverSocket.bind(('',serverPort))

message, clientAddress = serverSocket.recvfrom(256) #au cas ou le nom du fichier est bad long
if message.startswith(b"GET ") and message.endswith(b"\r\n\r\n"):
    nom_fichier = message[4:-4]
    fichier = nom_fichier.decode('utf-8')
    