#include<stdio.h>

#define ui unsigned int

int main(){
    ui a = 1, b = 2;
    printf("%u, %d\n" , a-b, (int)(a-b));
    return 0;
}