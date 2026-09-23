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
