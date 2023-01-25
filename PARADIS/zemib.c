#ifndef ZEMIB_H
#define ZEMIB_H
#include "zemib.h"
#endif

#include<stdio.h>

typedef unsigned int  ui;
typedef long long ll;

extern int size, seed;
ui mod, digMax;
ui bucket[8][MOD]; /*    bucket level */

void msdRadixSort(ui *array, ui argmod){
    mod = argmod;
    ui max = findMax(array);
    digMax = 0;
    ui powMod = mod;
    while(max>mod){
        max/=mod;
        digMax++;
    }
    radixSort(array, digMax, 0, size);
}

ui findMax(ui *array){
    ui max=0;
    for(int i=0;i<size;i++){
        if(max<array[i]) max = array[i];
    }
    return max;
}

void radixSort(ui *array, int l, int left, int right){

    ui head[mod], tail[mod], index;
    ui *lbucket = bucket[l];
    /*  make bucket empty */
    for(int i=0;i<8;i++)lbucket[i] = 0;
    /*  distribute array element to bucket  */
    for(int i=0;i<size;i++){
        /*  calculate l'th most significant digit to acindex with shift*/
        ui index = array[i] << (4*l);   /*  when mod=16, l'th digits of HEX begins at 4*(l-1)+1 bit of BIN : but l is 0-indexed so << 4*l */
        index = index >> 28; // 28 means 32-4, which is 4 most significant digits of previous acindex   
        lbucket[index]++;
    }

    /*  calculate head, tail of the indexes of the bucket*/
    head[0] = 0;
    tail[0] = lbucket[0]; 
    ui pocket = tail[0];
    for(int i=1;i<mod;i++){
        head[i] = pocket; 
        tail[i] = pocket + lbucket[i];
        pocket = tail[i];
    }

    /*  swap and permutate */
    for(int i=0;i<mod;i++){
        while(head[i]<tail[i]){
            ui v = array[head[i]];
            /*  calculate l'th most significant digit to acindex with shift*/
            index = v << (4*l);
            index = index >> 28; 
            while((index)!=i){
                ui tmp = array[head[index]]; 
                array[head[index]++] = v;
                v = tmp;
            }
            array[head[i]++] = v;
        }
    }

    ui prevtail = 0, curtail = 0;
    if(l--!=0){
        for(int i=0;i<mod;i++) {
            curtail = tail[i];
            if(curtail > prevtail) radixSort(array, l, prevtail, curtail); //if curtail = prevtail call is not required
            prevtail = curtail;
        }
    }
    return;
}
