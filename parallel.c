#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <stdbool.h>
#define ui unsigned int 
#define ll long long
#define POWMAX 31
#define MOD 1024

#define typeofdata ui

/*  O(size)を目指すデータ構造   */
struct Node {
    /* 枝分かれするまでデータ省略できるのでは？*/
}

/*  global for all programs */
int size, seed;

/*  build:  order for array -> order for tree   */
ui* build(ui *array){
    /* ここ工夫して部分木として表現し、出力の際に補正してやればデータ領域減らせそう*/  
    ll fixsize = (1LL<<33) -2;
    ui *cbt = (ui*)malloc(fixsize);
    for(ll i=0;i<fixsize;i++)cbt[i] = 0;

    cbt[0] = size;
    for(int i=0;i<size;i++){
        ui x = array[i];
        ll cp = 0;
        for(int k=31;k>=0;k--){
            if(x & (1<<k)){
                cp = 2*cp+2;
                cbt[cp]++;
            }
            else {
                cp = 2*cp+1;
                cbt[cp]++;
            }
        }
    }
    return cbt;
}

/*  build_parallel:  order for array -> order for tree   */
void build_parallel(ui *array){return;}
 
/* global variable for dfs  */
ui aryindex;
/*  dfs:    order for tree -> order forarray   */
void dfs(ll n, ui *array, int k, ui num){
    ui dfsnum=0;   
    bool leafflag = true;
    ll lp = 2*n+1, rp = 2*n+2;
    if(array[lp]!=0) {
        printf("dfs(%llu)\n", n);
        dfs(lp, array, k+1, num);
        leafflag = false;
    }
    if(array[rp]!=0){
        dfsnum = num + (1<<(POWMAX-k));
        printf("dfs(%llu)\n", n);
        dfs(rp, array, k+1, dfsnum);
        leafflag = false;
    }
    if(leafflag){
        ui cnt = array[n];
        printf("cnt = %u\n", cnt);
        while(cnt-->0){
            array[aryindex] = num;
            aryindex++;
        }
    }
    return;
} 




double sort1(ui *array){
    double time;

    int i;

    time = omp_get_wtime();
    //逐次ソート開始
    ui *cbt = build(array);
    printf ("build (%lf)\n", omp_get_wtime()-time);
    aryindex = 0;
    dfs(aryindex, cbt, 0, 0);
    printf ("dfs (%lf)\n", omp_get_wtime()-time);
    //逐次ソート終了
    time = omp_get_wtime() - time;

    int flag = 0;
    for(i=0; i<size-1; i++){
        if((int)(array[i+1] - array[i]) < 0)
            flag = 1;
    }

    if(flag) {
        printf("Step program may wrong.\n");
        return -1;
    }
    return time;
}

double sort2(ui *array){
    double time;

    int i;

    time = omp_get_wtime();
    //並列ソート開始
    //並列ソート終了
    int flag = 0;
    for(i=0; i<size-1; i++){
        if((int)(array[i+1] - array[i]) < 0)
            flag = 1;
    }

    if(flag) {
        printf("Parallel program may wrong.\n");
        return -1;
    }
    time = omp_get_wtime()-time;
    return time;
}

void main(int argc, char *argv[])//プログラム名 大きさ シード値
{
    if(argc != 3) {
        printf("please input size and seed\n");
        return;
    }
    size = atoi(argv[1]);
    seed = atoi(argv[2]);
    srand(seed);

    unsigned int *array1, *array2;
    array1 = (ui*) malloc (sizeof(ui)*size);
    array2 = (ui*) malloc (sizeof(ui)*size);

    int i;
    for(i=0; i<size; i++){
        array1[i] = rand()%MOD;
        array2[i] = array1[i];
    }

    double time1 = sort1(array1);
    double time2 = sort2(array1);

    printf("time1 = %lf, time2 = %lf\n", time1, time2);
    printf("speedup = %lf\n", time1 / time2);

    free(array1);
    free(array2);
}