int main(){

//question 2
//prends le contenu de from et le met dans to
    int f(int* from, int* to, int count){
        for(int i = 0; i<count; i++;){
            to[i] = from[i];
        }
    }

    //question 3
    #define MAX 1
    struct x {
        int x_a;
        int x_b;
    } t[MAX];

    struct x* p;
    int cpt = 0;
    for(p = t; p < &t[MAX], p++){
        cpt =+ p->x_a
    }
}

//q4
