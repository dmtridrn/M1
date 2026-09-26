from socket import *
import random
serverPort = 1234
serverSocket = socket(AF_INET,SOCK_DGRAM)
serverSocket.bind(('',serverPort))
print('server ready')
while True:
    message, clientAddress = serverSocket.recvfrom(2048)
    if random.randint(1, 10) <= 5:  #50% de chance d'ignorer
        continue
    print(message.decode('utf-8'))
    modifiedMessage = message.upper() #on envoie n'importe la juste on répète en criant
    serverSocket.sendto(modifiedMessage,clientAddress)