#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include "mtrand.h"

void work (int index) {
    std::cout << "started "<< index << std::endl;
    auto r = mtrand(0,1000); // 0 to 999 msec
    std::this_thread::sleep_for (std::chrono::milliseconds(r));
    std::cout << "finished "<< index << std::endl;
}

void createAndWait(int N){
    std::vector<std::thread> workers;
    for(int i = 0; i < N; ++i){
        workers.emplace_back(work, i);
    }

    for(auto& t : workers){
        t.join();
    }
}

int main (int argc, char ** argv) {
    int N = 3;
    if (argc > 1) {
    N=atoi(argv[1]);
    }
    createAndWait(N);
    return 0;
}

/*
Question 8:
&c (capture par référence) : tous les threads partagent l'accès à la même et unique instance de Compte 
présente sur la pile du main.   
i (capture par valeur/copie) : chaque thread reçoit sa propre copie immuable de l'index au moment de son instanciation.   

Pourquoi [&] est un désastre : si on capturait tout par référence, tous les threads pointeraient vers
 la même variable locale i de la boucle. Pire encore, la boucle peut se terminer bien avant les threads
  : la variable i sur la pile est alors détruite (dangling reference), ce qui mène à un Undefined Behavior (UB) immédiat.
*/

/* 
Question 12:
Inutilité de l'atomic avec le mutex :
Comme toutes les méthodes (crediter, debiter, getSolde) sont déjà mutuellement exclusives grâce au mutex, 
les accès concurrents sont déjà éliminés. Garder atomic n'apporte aucune garantie supplémentaire et ajoute un 
surcoût matériel inutile (barrières mémoire / instructions atomiques superflues). Un simple int suffit.   

Nécessité absolue du mutex sur getSolde sans atomic :
En C++, une data race survient dès que deux accès concurrents ont lieu sur une même adresse mémoire, dont 
au moins un en écriture, sans synchronisation. Si getSolde lisait solde sans prendre le verrou pendant qu'un autre 
thread est en train d'exécuter solde += val ou solde -= val, c'est un accès concurrent lecture/écriture non 
synchronisé : c'est un Undefined Behavior (UB) selon la norme C++. Le mutex est donc obligatoire pour lire une 
valeur cohérente et respecter le modèle mémoire.
*/