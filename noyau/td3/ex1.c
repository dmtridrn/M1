#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void bonjour(){
    printf("bonjour\n");
    alarm(1);
    signal(SIGALRM, bonjour);
}

int main(){
    signal(SIGALRM, bonjour);
    alarm(1);
    while (1){
        pause();
    }
}