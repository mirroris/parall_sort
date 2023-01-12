#include<stdio.h>

#define ui unsigned int

int solve(int a, int b){
    a++;
    b++;
    return a+b;
}


int main(){
    int a = 1, b = 2;
    printf("%d\n",solve(a,b));
    return 0;
}