#pragma once
#include <cstddef>
#include "strutil.h"

namespace pr{
Class String{
    const char * str;
public:
    String(const char *cstr=""): str(newcopy(cstr)){}
    ~String(){delete[] str}
    size_t length() const {return pr::length(str)}
};
}


int main(){
    String abc = "abc";
    {
        String bcd(abc); 
    }//delete le pointeur et ce qu'il pointe sur le tas 
     //(donc "abc" qui était aussi pointé par abc)

    std::cout << abc << std::endl; //A

    String def = "def";
    def = abc;

    std::cout << abc << " et " << def << std::endl;//B
}

/*
Q1: le littéral "abc" sur le tas s'est fait delete par le destructeur de bcd donc crash
    sur A et sur B
Q2: 
*/
String(const String& other): str(newcopy(other.str)){}
String& operator=(const String& other){
    if(this != &other){
        delete[] str;
        str = newcopy(other.str);
    }
    return *this;
}
/*
Q3: un contructeur classique, un contrsucteur par copie, 
une opération d'affectation, un destructeur, un contructeur de déplacement,
affectation par déplacement

Q4: l15: empty, l16-17: push_back, l18: push_front, l6-7: size(), 
l8(r)-20(w): operator[], l14: constructeur par défaut, destructeur, dans push back: reserve 
Q5:
*/

template<typename T>
class Vector{
    size_t size;
    size_t alloc_size;
    T* tab;

public:
    Vector(int nbel=0): size(nbel), alloc_size(0), tab(nullptr){
        reserve(size);
        int i;
        for(i = 0; i<nbel; i++){
            tab[i] = T();
        }
    }

    ~Vector(){
        delete[] tab;
    }

    size_t size() const {return size;}

    void reserve(int n){
        if(n > alloc_size){
            alloc_size = std::max(alloc_size * 2, n); //pour le cas size = 0
            T* new_tab = new T[alloc_size];
            for(int i = 0; i<size, ++i){
                new_tab[i] = tab[i];
            }
            delete[] tab;
            tab = new_tab;
        }
    }

    void push_back(const T& val){
        reserve(size+1);
        tab[size++] = val;
    }
    void push_front(const T& val){
        reserve(size+1);
        for(size_t i = size; i>0; i--){
            tab[i] = tab[i-1]
        }
        tab[0] = val;
        size++;
    }
    T& operator[](size_t index){
        return tab[index];
    }
    void affiche(const Vector<string>& vec){
        
    }
}