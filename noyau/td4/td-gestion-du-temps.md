**Q1 Notion de temps**



Matériel

Horloge temps réel maintenue par le bios. 

Composant horloge en parallèle du CPU avec sa fréquence :

RH--

if RH == 0 lever IT Horloge => execution dans le système

tick = temps entre 2 IT horloges => fixer par un parametre HZ



**Sysèmes** 

Heure : recopiée depuis le BIOS, mise à jour par le système dans une variable global **time** nombre de seconde depuis 1er janvier 1970

temps cpu des processus (commande times) : nb de ticks consommé en mode u (**u_utime**), en mode s (**u_stime**).



**Ordonnancement et swapper**

p_cpu (tick consommés), p_time (temps de résidence en mémoire) 



**Q2 timeout**

mécanisme interne utilisé par les drivers pour exécuter une fonction dans le noyau (pas de signal) au de x tick



Attention différent de alarm(y) : appelé par un processus => après y secondes envoyer un SIGALRM au processus appelant



timeout(f,arg, x) => après x ticks f(arg) appélée dans le noyau à un prio CALOUT



1 table pour stocker les timeout en cours (callout[NCALL))

Stocker des c_time en **relatif par rapport au timeout précédent**



|            |            |             |
| ---------- | ---------- | ----------- |
| c-func     | c_arg      | c_time      |
| f1         | NULL       | 10 (1)      |
| f2         | NULL       | 5   (2)     |
| f3         | NULL       | 5 (3)       |
| f4         | NULL       | 10 (4)      |
| f6         | NULL       | 6           |
| **NULL**   |            | **Bouchon** |
|            |            |             |

timeout(f1, NULL, 10);

timeout(f2, NULL, 15);

timeout(f3, NULL, 20);

timeout(f4, NULL, 30);

timeout(f6,NULL, 36)



**Q3 Algorithme de timeout**



Masque IT Horloge

Recherche l'emplacement de la fonction

Décrémenter le temps de l'entrée suivant

*Décalage du tableau vers le bas :*

Rechercher le "bouchon" (p2)

Si bouchon est le dernier élement

panic (tableau est plein)

Décaler d'un cran le tableau vers le bas

Mettre à la nouvelle entrée

Demaster les IT



timeout(f5,NULL, 22);



t=22;

(1) t=22 > c_time(10)  => t=t-10 = 12

(2) t=12 > c_time(5) => t=t-5 = 7

(3) t=7 > c_time(5) => t=t-5 = 2

(4) t=2 < c_time => sortir



|            |            |             |
| ---------- | ---------- | ----------- |
| c-func     | c_arg      | c_time      |
| f1         | NULL       | 10          |
| f2         | NULL       | 5           |
| f3         | NULL       | 5           |
| f5         | NULL       | **2**       |
| f4         | NULL       | **8**       |
| f6         | NULL       | 6           |
| **NULL**   |            | **Bouchon** |
|            |            |             |

delay ? 

Endort le processus courant à un priorité non interruptible pendant "ticks" ticks.



**Q4 untimeout**

|            |            |            |            |
| ---------- | ---------- | ---------- | ---------- |
| c_func     | c_arg      | c_time     | **c_id**   |
| f1         | NULL       | 10         | 23         |
| f2         | NULL       | 5          | 44         |
| **f3**     | **NULL**   | **3**      | **65  p1** |
| f4         | NULL       | **2**      | 34         |
| f5         | NULL       | 10         | 12         |
| f6         | NULL       | 5          | 76         |
| NULL       |            |            |            |

untimeout(65)



```c
void untimeout(int ident) {
struct callo *p1;
int ps;
/* Masquer IT Horloge */
ps = gpl();
spl(CLINBH);

/* rechercher l'identifiant */
p1 = &callout[0];
while (p1->c_func != 0 && p1->c_id != ident)
  p1++;

/* pas trouvé */
if (p1->c-func ==0) {
  spl(ps);
  return;
}
/* mise à jour de l'identifiant suivant */
if ((p1+1)->c_func != 0) 
  (p1+1)->c_time += p1->c_time;

/* décalage d'un cran vers le haut */

while (p1->c_func != 0) {
  p1->c_time = (p1+1)->c_time;
  p1->c_func = (p1+1)->c_func;
  p1->c_arg = (p1+1)->c_arg;
  p1->c_cid = (p1+1)->c_cid;
  p1++;
}

spl(ps);

}




```





|            |            |            |            |
| ---------- | ---------- | ---------- | ---------- |
| c_func     | c_arg      | c_time     | **c_id**   |
| f1         | NULL       | 9          | 23  p1     |
| f2         | NULL       | 5          | 44         |
| **f3**     | **NULL**   | 3          | **65  p1** |
| f4         | NULL       | **5**      | 34         |
| f5         | NULL       | 10         | 12         |
| f6         | NULL       | 5          | 76         |
| NULL       | -          | -          | **p2**     |



**Q5 Interruption horloge**



HZ : nombre de fois par seconde qu'est déclenché l'IT horloge

HZ = 60 



IT horloge => assembleur

```c
/* pour les 1/H2 seconde (IT Horloge) */
clock();
if (iaflags&CALOUT)
  restart();   /* 1 timeout expiré */
if (iaflags&WAKEUP)
  realtime();   /* 1 seconde écoulé => commuté */
```



Algorithme de clock :

**clock**()  IT horloge déjà masquée



Repositionner le registre horloge  à la valeur du tick

Trouver la première valeur à c_time non vide et la décrémenter

Si le processeur n'est pas oisif (pas idle)

 incrémenter le p_cpu  du processus courant jusqu'a un max 

Si interrompu en mode U

incrémenter le nombre de tick consommé en mode U par le processus courant 

sinon 

si processeur non oisif

incrémenter le nombre de tick consommé en mode S par le processus courant

Positionner le flag CALOUT si une fonction à appeler dans callout => appel à restart

Positioner le flag WAKUP si un seconde s'est écoulée => appel à realtime



**realtime** ()   Appelée toutes les secondes



Mise à jour de l'heure (+1 seconde)

Positionner runrun pour forcer un election (swtch) lors du prochain passage en mode U

[Réveiller les drivers en attente sur lbolt]

Pour tous les processus ayant une entrée dans proc[]

Augmenter le temps de résidence en mémoire ou swap (p_time)

Ai le processus a demandé une alarme et que cette alarme est atteinte

envoyer le signal SIGALRM   (appel systeme alarm(v) ) 

Diviser par 2 le p_cpu du processus

Si le processus à une priorité utilisateur

recalculer sa priorité en fonction p_cpu

Réveiller le swapper s'il est en attente de mémoire (tous les processus ont vieilli d'une seconde)







Priorité des processus

|              |            |            |             |                   |
| ------------ | ---------- | ---------- | ----------- | ----------------- |
| +prioritaire | PSWAP      | -100       | Système     | Non interruptible |
|              | ..         |            |             |                   |
|              | ...        |            |             |                   |
|              | PZERO      | 0          | Système     |                   |
|              | ...        |            |             | Interruptible     |
|              | PUSER      | 100        | Utilisateur |                   |
|              |            |            |             |                   |
|              |            |            |             |                   |
| -prioritaire |            | 127        |             |                   |



**Q6 restart**



|            |            |             |
| ---------- | ---------- | ----------- |
| c-func     | c_arg      | c_time      |
| f1         | NULL       | **0  p2**   |
| f2         | NULL       | 0           |
| f3         | NULL       | 0           |
| f5         | NULL       | **2 p1**    |
| f4         | NULL       | **8**       |
| f6         | NULL       | 6           |
| **NULL**   |            | **Bouchon** |

timeout(f1,NULL, 10);

timeout(f2,NULL, 10);

timeout(f3,NULL, 10);



```c
restart() { /* It Horloge */
  register struct *p1,*p2;
  
  /* Demasquer l'IT Horloge pour exécuter les fonctions */
  spl(CALOUT);
  p1=&callout[0];
  /* Exécuter les fonctions */
  while (p1->c_func != 0 && p1->c_time <= 0) {
    (*p1->c_func)(p1->c_arg); /* Appel */
    p1++
  }
  
  /* Décalage du tableau vers le haut */

  /* Remasquer l'IT horloge */
  spl(CLINHB);
  p2=&callout[0];
  while ( (p2->c_func = p1->c_func) != NULL) {
    p2->c_time = p1_>c_time;
    p2->c_arg = p1 ->c_arg;
    p1++;
    p2++
  }
}
```





Q7 **setpri**



appelée par realtime pour chaque processus de priorité >= PUSER



setpri (p) 

priorité de p = min (127, PUSER + p_cpu / 16 + valeur du nice)

si priorité de p inférieur au processus courant (ie., p plus prioritaire)

positionner runrun pour forcer forcer une election lors du passage en mode U





int t[100];



int *p= &t[0];



printf("...",p, p+1 )
