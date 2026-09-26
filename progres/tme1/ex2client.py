from socket import *
import time
serverName = '192.168.1.15'
serverPort = 1234
clientSocket = socket(AF_INET,SOCK_STREAM)
clientSocket.connect((serverName,serverPort))


message = 'a'.encode('utf-8')
while True: #on envoie jusqu'à la mort
    time.sleep(1)
    clientSocket.send(message)
    modifiedMessage = float(clientSocket.recv(2048).decode('utf-8'))
    temps = time.time()

    print(f"temps local: {temps}/ temps distant: {modifiedMessage}/ décalage {abs(temps-modifiedMessage)}") #on calcule juste le décalage
