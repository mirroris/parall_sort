#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int size, seed;
void shuffle(int *array);

double sort1(int *array){
    double time;

    int i;

    time = omp_get_wtime();
    //逐次ソート開始
    /*
    while (1)
    {
        int flag=0;
        for(i=0; i<size-1; i++){
            if(array[i] > array[i+1])
                flag = 1;
        }
        if(!flag) break;

        shuffle(array);
        flag = 0;
    }
    */
    //逐次ソート終了
    time = omp_get_wtime() - time;

    int flag = 0;
    for(i=0; i<size-1; i++){
        if(array[i+1] - array[i] < 0)
            flag = 1;
    }

    if(flag) {
        printf("Step program may wrong.\n");
        return -1;
    }
    return time;
}

double sort2(int *array){
    double time;

    int i;

    time = omp_get_wtime();
    //並列ソート開始
    #pragma omp parallel
    {
        //ボゴソートの並列化は諦めました
    }
    //並列ソート終了
    int flag = 0;
    for(i=0; i<size-1; i++){
        if(array[i+1] - array[i] < 0)
            flag = 1;
    }

    if(flag) {
        printf("Parallel program may wrong.\n");
        return -1;
    }
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
    array1 = (int*) malloc (sizeof(int)*size);
    array2 = (int*) malloc (sizeof(int)*size);

    int i;
    for(i=0; i<size; i++){
        array1[i] = rand();
        array2[i] = array1[i];
;    }

    double time1 = sort1(array1);
    double time2 = sort2(array2);

    printf("time1 = %lf, time2 = %lf\n", time1, time2);
    printf("speedup = %lf\n", time1 / time2);

    free(array1);
    free(array2);
}

void shuffle(int *array){
    for(int i=0; i<size; i++){
        int j = rand()%size;
        int keep = array[i];
        array[i] = array[j];
        array[j] = keep;
    }
}