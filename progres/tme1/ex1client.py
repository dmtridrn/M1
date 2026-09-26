from socket import *
import time
serverName ='10.51.22.74'
serverPort = 1234
clientSocket = socket(AF_INET,SOCK_DGRAM)
message = 'ping'.encode('utf-8')
totaltemps = 0
totalreponses = 0
clientSocket.settimeout(1.0) #si le serveur oublie de répondre
for i in range(10):
    debut = time.time() #temps au moment de l'envoi
    clientSocket.sendto(message,(serverName,serverPort))
    try:
        modifiedMessage, serverAddress = clientSocket.recvfrom(2048)
        fin = time.time() #temps au moment de la réception
        print(modifiedMessage.decode('utf-8') + f" temps: {fin-debut}")
        totaltemps += (fin-debut)
        totalreponses += 1
    except timeout:
        print("pas de réponse (timeout)")
    time.sleep(1)

if totalreponses > 0:
    print(f"moyenne : {totaltemps / totalreponses} s ({totalreponses}/10 reçus)")
else:
    print("rien reçu")