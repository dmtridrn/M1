#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>

int main(){
    int N = 4;
    for(int i = 0; i<N; ++i){
        if(!fork()){
            std::cout << "PID: " << getpid() << ", PID Père: " << getppid() << ", Numéro: " <<  i << std::endl;
            exit(i);
        }
    }
    for(int j = 0; j<N; ++j){
        int ret;
        pid_t pid_fils = wait(&ret);
        if(WIFEXITED(ret)){
            std::cout << "Fils pid: " << pid_fils << " exit status: " << WEXITSTATUS(ret) << std::endl;
        }
    }
}