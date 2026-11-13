#include <vector>
#include <thread>
#include <iostream>
#include <atomic>
#include <mutex>
#include <random>
#include "mtrand.h"

using namespace std;

class Compte
{
    mutable recursive_mutex m;
    int solde;

public:
    Compte(int solde = 0) : solde(solde) {}
    void crediter(int val)
    {
        lock_guard g(m);
        solde += val;
    }
    bool debiter(int val)
    {
        lock_guard g(m);
        bool doit = solde >= val;
        if (doit)
        {
            solde -= val;
        }
        return doit;
    }
    int getSolde() const
    {
        lock_guard g(m);
        return solde;
    }
    void lock() const
    {
        m.lock();
    }
    void unlock() const
    {
        m.unlock();
    }

    // NB : vector exige Copyable, mais mutex ne l’est pas...
    Compte(const Compte &other)
    {
        lock_guard g(other.m);
        solde = other.solde;
    }
};

class Banque
{
    using comptes_t = vector<Compte>;
    comptes_t comptes;

public:
    Banque(size_t ncomptes, size_t solde) : comptes(ncomptes, Compte(solde))
    {
    }

    void transfert(size_t deb, size_t cred, unsigned int val)
    {
        Compte &debiteur = comptes[deb];
        Compte &crediteur = comptes[cred];
        lock_guard l1(comptes[min(deb, cred)]);
        lock_guard l2(comptes[max(deb, cred)]);
        if (debiteur.debiter(val))
        {
            crediteur.crediter(val);
        }
    }

    size_t size() const
    {
        return comptes.size();
    }

    bool comptabiliser(int attendu) const
    {
        int bilan = 0;
        int id = 0;
        for (const auto &compte : comptes)
        {
            compte.lock();
            if (compte.getSolde() < 0)
            {
                cout << "Compte " << id << " en négatif : " << compte.getSolde() << endl;
            }
            bilan += compte.getSolde();
            id++;
        }
        if (bilan != attendu)
        {
            cout << "Bilan comptable faux : attendu " << attendu << " obtenu : " << bilan << endl;
        }
        for (const auto &compte : comptes){
            compte.unlock();
        }
        return bilan == attendu;
    }
};


int main()
{
    size_t SOLDEINITIAL = 100;
    size_t K = 10;
    Banque b{K, SOLDEINITIAL};
    vector<thread> threads;
    cout << "transfert commencé" << endl;
    for (int i = 0; i < 100; ++i)
    {
        threads.emplace_back([&b]()
                             {
            for(int j = 0; j<100; ++j){
                auto x = mtrand(0,b.size());
                auto y = x;
                while (y == x)
                {
                   y = mtrand(0,b.size());
                }
                b.transfert(x,y, rand()%100);
                cout << "transfert: " << x << " vers " << y << endl;
            } });
    }
    for (auto &t : threads)
    {
        t.join();
    }
}