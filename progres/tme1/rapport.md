# Rapport tme1

## Exercice 1

Pour cet exercice, le serveur est très simple: il reçoit un message et répond à la même addresse dans une boucle infinie.
Le client envoie un message au serveur et attend sa réponse pour mesurer le temps entre l'envoi et la réception.
Avec une probabilité que le serveur ne réponde pas à la moitié des messages, j'ai mis un timeout (`clientSocket.settimeout(1.0)`) sur la socket côté client pour ne pas rester bloqué sur le recvfrom en plus d'un compteur des réponses. Ainsi, la moyenne calculée est celle de l'attente de réponse des paquets efectivement reçus.
```python
print(f"moyenne : {totaltemps / totalreponses} s ({totalreponses}/10 reçus)")
```
Pas besoin de Thread ni Poll pour gérer plusieurs clients: udp n'impose pas de connection avec le serveur, il peut donc répondre à n'importe quelle addresse qui lui a envoyé un message.

## Exercice 2

La grande différence avec l'exercice 1 est que la discussion client serveur se fait ici en TCP. Un serveur ne peut donc pas gérer plusieurs clients en même temps sur une même socket.
J'ai donc décidé d'implémenter un serveur multithread capable de palier à ce problème:
Le serveur accepte des nouveaux clients dans une boucle infinie et crée une socket pour leur comminication, il lance ensuite un Thread avec ce socket en argument (`Thread(target=handle_client,args=(connectionSocket,)).start()`). Ce Thread va en boucle:
- recevoir le message du client
- lui envoyer son heure locale
- fermer la connection (si le client coupe la communication)  

Le client ne fait rien de spécial, il envoie un message au serveur, reçoit une réponse et compare son heure locale avec celle du serveur

## Exercice 3
Pour cet exercice j'ai voulu essayer d'implémenter une notion du cours que je connaissais déjà en C: le select/poll. Un serveur multithread aurait très bien fonctionné mais je trouve poll plus élégant. Je me suis basé sur le squelette vu en cours (4 branches, dictionnaires to_send, received...) pour l'implémentation.  

Après avoir créé l'objet poll, le serveur instancie 3 dictionnaires:  
- un dictionnaire sockets FileDescriptor:SocketObject (car poll renvoie un fd et non un objet socket, on doit donc garder une trace de qui est qui)
- un dictionnaire received fd:data qui stocke les données reçues sur un socket fd
- un dictionnaire to_send fd:data qui stocke les données à envoyer sur un socket fd  

Ces 2 derniers dictionnaires seront utiles si une réception ne capte pas tout le message d'un coup ou si un envoi n'est pas total.  

Quand poll détecte un évènement, il renvoie un tuple (fd,event); il est possible de savoir quel(s) évènement(s) ont eu lieu grâce au masquage binaire (POLLHUP | POLLERR) et au filtrage (event & masque). On peut ainsi définir un comportement pour chaque cas qui nous intéresse.

J'ai aussi décidé de créer un fichier par défaut `index.html` au cas où le client ne fais pas de demande de fichier explicite dans sa reqête HTML (comme on accèderai à une page d'accueil d'un site sans fichier après l'url).

Pour tester plusieurs clients faisant plusieurs requête en simultané, jai utilisé apache benchmark pour simuler un scénario pouvant mettre mon serveur à l'épreuve (100 clients et 1000 requêtes en tout):  
```bash
ab -n 1000 -c 100 http://127.0.0.1:1234/index.html
```

## Globalement

La version actuelle du code des exercices 1 et 2 est faite pour tester en local (ip 127.0.0.1) mais des tests sur plusieurs machines sur un même sous-réseau en simultané ont aussi été réalisés.

Pour la gestion des erreurs réseau, je ne catch que OSError car aucun besoin de définir un comportement différent pour BlockingIOError, ConnectionResetError ou BrokenPipeError par exemple.


