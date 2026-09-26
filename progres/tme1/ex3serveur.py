# j'ai utilisé poll même si loin d'être nécessaire j'aime bien

from socket import *
from select import *
from pathlib import Path

#setup la socket
serverPort = 1234
serverSocket = socket(AF_INET,SOCK_STREAM)
serverSocket.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1) #pour address already in use 
serverSocket.bind(('',serverPort))
serverSocket.listen(67)
serverSocket.setblocking(False) #si recv bloque ca paralyse pas tout

#setup de poll et écoute sur la socket serv
my_poll = poll()
my_poll.register(serverSocket,POLLIN)

sockets = {serverSocket.fileno(): serverSocket} #dict fd:objet_sock
received = dict() #buffer recep
to_send = dict() #buffer envoi

fichier_defaut = "index.html" #si on nous demande rien en particulier

#petit nettoyage
def clean_socket(fd):
    my_poll.unregister(fd)
    received.pop(fd, None)
    to_send.pop(fd, None)
    sock = sockets.pop(fd, None)
    if sock:
        sock.close()

#boucle d'écoute du poll
while True:
    for fd,event in my_poll.poll():
        if event & (POLLHUP|POLLERR|POLLNVAL):
            clean_socket(fd)
        elif event & POLLIN: #on détecte une entrée
            if sockets[fd] == serverSocket: #si c le sock serveur => nouvelle conection
                clientSocket, address = serverSocket.accept()
                clientSocket.setblocking(False)
                sockets[clientSocket.fileno()] = clientSocket
                my_poll.register(clientSocket,POLLIN)
            else: #client
                data = sockets[fd].recv(4096)
                if not data:
                    clean_socket(fd)
                    continue
                received[fd] = received.get(fd, b'') + data
                if b"\r\n\r\n" in received[fd]: #si la requête est complète
                    requete_str = received.pop(fd).decode('utf-8')
                    premiere_ligne = requete_str.splitlines()[0]
                    elts = premiere_ligne.split()
                    if elts[0] != "GET" or len(elts) < 3: #pas une requête http / pas la bonne
                        clean_socket(fd)
                        continue
                    chemin = elts[1].lstrip('/')
                    if chemin == "":
                        chemin = fichier_defaut
                    fichier = Path(chemin)
                    if fichier.is_file(): #si on trouve le fichier on envoie code 200
                        with open(fichier, 'rb') as f:
                            contenu = f.read()
                        entete = ( #source: https://fr.wikipedia.org/wiki/Hypertext_Transfer_Protocol
                            "HTTP/1.1 200 OK\r\n"
                            "Content-Type: text/html\r\n"
                            f"Content-Length: {len(contenu)}\r\n"
                            "\r\n"
                        ).encode('utf-8')
                        to_send[fd] = entete + contenu
                        my_poll.modify(fd, POLLOUT)
                    else: #sinon envoie 404 error
                        contenu = b"<html><body><h1>erreur 404 fichier not found </h1></body></html>" #juste un message d'erreur
                        entete = (
                            "HTTP/1.1 404 Not Found\r\n"
                            "Content-Type: text/html\r\n"
                            f"Content-Length: {len(contenu)}\r\n"
                            "\r\n"
                        ).encode('utf-8')
                        to_send[fd] = entete + contenu
                        my_poll.modify(fd, POLLOUT)
        elif event & POLLOUT: #si ya qqch a envoyer
            if fd in to_send:
                data = to_send[fd]
                n = sockets[fd].send(data)
                if n < len(data): #si on envoie pas tout
                    to_send[fd] = data[n:] #pas grave on stock ce qui reste à envoyer
                else: #tout est OKKKKK
                    clean_socket(fd)

# testé avec apache benchmark en vif (simule 10 clients qui font 10 requêtes chacun en simultané)
# ab -n 100 -c 10 http://127.0.0.1:1234/index.html