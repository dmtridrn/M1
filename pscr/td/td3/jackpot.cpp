#include <vector>
#include <thread>
#include <iostream>
#include <atomic>
#include <mutex>


using namespace std;
class Compte {

    mutable std::mutex mtx;
    std::atomic<int> solde;

    public :
        Compte(int solde=0):solde(solde) {}
        Compte(const Compte& other) {
            std::lock_guard lock(other.mtx);
            this->solde = other.solde;
        }

        void crediter (int val) {
            std::lock_guard lock(mtx);
            solde+=val;
        }

        int getSolde() const {
            std::lock_guard lock(mtx);
            return solde;
        }

        bool debiter(int val) {
            std::lock_guard lock(mtx);
            bool doit = (solde >= val);
            if (doit) {
                solde -= val;
            }
            return doit;
        }
};

const int JP = 10000;
void jackpot(Compte & c) {
    for (int i=0; i < JP; i++){
        c.crediter(1);
    }
}
void losepot(Compte & c) {
    for (int i=0; i < JP / 10; i++){
        c.debiter(10);
    }
}

const int NB_THREAD = 10;
int main () {
    vector<thread> threads;
    threads.reserve(NB_THREAD);
    // TODO : creer des threads qui font jackpot sur un compte
    Compte c;
    for(int i = 0; i<NB_THREAD; ++i){
        threads.emplace_back(jackpot, std::ref(c));
    }
    for (auto & t : threads) {
        t.join();
    }
    std::cout << c.getSolde() << std::endl;
    // TODO : tester solde = NB_THREAD * JP
    return 0;
}