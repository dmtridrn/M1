(1)
size_t length(const char* c)
char* newcopy(const char* c)

(2)
strutil.h
#pragma once
namespace pr{
    size_t length(const char* c)
    char* newcopy(const char* c)
}

(3)
size_t length(const char* c){
    size_t cpt = 0;
    for(size_t i = 0; c[i]!='\0'; i++){
        ++cpt;
    }
    return cpt;
}

size_t length(const char* c){
    size_t cpt = 0;
    while(*(c++) != '\0'){
        cpt ++;
    }
    return cpt;
}
placer dans le .cpp

(4)
char* newcopy(const char* c){
    size_t len = length(c);
    char* dest = (char*)malloc(len+1);
    for(size_t i = 0; i<=len; i++){
        dest[i] = c[i];
    }
    return dest;
    free(dest);
}

char* newcopy(const char* c){
    size_t len = length(c);
    char* dest = new char[len+1];
    for(size_t i = 0; i<=len; i++){
        dest[i] = c[i];
    }
    return dest;
    delete[] dest;
}

(6)
char* newcopy(const char* c){
    size_t len = length(c);
    char* dest = new char[len+1];
    memcpy(dest, c, len+1);
    return dest;
}

(7)
using namespace std
int main(){
    const char* mot = "Hello World";
    char* copie = newcopy(mot);
    cout << mot << "-" << copie << endl;
    cout << (void*)mot << "-" << (void*)copie << endl;
    cout << length(mot) << "-" << length(copie) << endl;
    delete[] copie;
}

litteral dans rodata
copie dans le tas
variables dans la pile

(8)
g++ -c strutil.cpp -o strutil.o -std=c++20 -g -O0
g++ -c exo1.cpp -o exo1.o -std=c++20 -g -O0
g++ strutil.o exo1.o -o exo1

(9)

