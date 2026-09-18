from socket import *
import time
serverName ='10.51.22.74'
serverPort = 1234
clientSocket = socket(AF_INET,SOCK_DGRAM)
message = 'ping'.encode('utf-8')
totaltemps = 0
for i in range(10):
    debut = time.time()
    clientSocket.sendto(message,(serverName,serverPort))
    modifiedMessage, serverAddress = clientSocket.recvfrom(2048)
    fin = time.time()
    print(modifiedMessage.decode('utf-8') + f" temps: {fin-debut}")
    totaltemps+=fin-debut
    time.sleep(1)

print(totaltemps/10)