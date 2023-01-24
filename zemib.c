#ifndef ZEMIB_H
#define ZEMIB_H
#include "zemib.h"
#endif

#include<stdio.h>

typedef unsigned int  ui;

extern int size, seed;
ui mod, digMax;
ui bucket[8][MOD]; /*    bucket level */

void msdRadixSort(ui *array, ui mod){
    ui max = findMax(array);
    digMax = 1;
    ui powMod = mod;
    while(max>=powMod){
        powMod=powMod*mod;
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

void radixSort(ui *array, ui l, ui left, ui right){
    ui head[mod], tail[mod];
    ui *lbucket = bucket[l];
    /*  make bucket empty */
    for(int i=0;i<8;i++)lbucket[i] = 0;
    /*  distribute array element to bucket  */
    ui divmod = 1;
    for(int i=0;i<l;i++)MOD*divmod;
    for(int i=0;i<size;i++){
        lbucket[array[i]%divmod]++;
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
            ui acindex = v%divmod;
            while((acindex)!=i){
                ui tmp = array[head[acindex]]; 
                array[head[acindex]++] = v;
                v = tmp;
            }
            array[head[i]++] = v;
        }
    }
    printf("radixSort(arra, %u, ...)\n",l);
    if(--l!=0){
        for(int i=0;i<mod;i++) {
            if(i==0) radixSort(array, l, 0, tail[i]);
            else radixSort(array, l, tail[i-1], tail[i]);
        }
    }
    return;
}
