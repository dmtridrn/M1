from socket import *
import random
serverPort = 1234
serverSocket = socket(AF_INET,SOCK_DGRAM)
serverSocket.bind(('',serverPort))
print('server ready')
while True:
    message, clientAddress = serverSocket.recvfrom(2048)
    flag = random.randint(1,2)
    print(message.decode('utf-8'))

    modifiedMessage = message.upper()
    serverSocket.sendto(modifiedMessage,clientAddress)