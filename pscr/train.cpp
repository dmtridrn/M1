#include <iostream>
#include <string>

void h(int& i){
    i = 30;
}

int main() {
    int a = 100;
    h(a);
    std::cout << a << '\n';
}
