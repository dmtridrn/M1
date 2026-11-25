#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>

int main(){
    int N = 4;
    for(int i = 0; i<N; ++i){
        if(!fork()){
            std::cout << "PID: " << getpid() << ", PID Père: " << getppid() << ", Numéro: " <<  i << std::endl;
            exit(0);
        }
    }
    for(int j = 0; j<N; ++j){
        wait(nullptr);
    }
}