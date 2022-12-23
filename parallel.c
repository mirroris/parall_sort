#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include<stdbool.h>
#define ui unsigned int 
#define POWMAX 31
#define MOD 1024

/*  global for all programs */
int size, seed;
struct Node{
    ui num;
    struct Node *lp;
    struct Node *rp;
};
typedef  struct Node Node;



/*  build:  order for array -> order for tree   */
void build(Node *np, ui *array){
    for(int i=0; i<size;i++){
        ui x = array[i];
        printf("x = %u\n = ",x);
        for(int k=31;k>=0;k--){
            if(((1<<k)& x)!=0){
                printf("1");
                if(np->rp==NULL){
                    Node *child = (Node *)malloc(sizeof(Node));
                    child->rp = NULL;
                    child->lp = NULL;
                    np->rp = child;
                }
                (np->num)++;
                np = np->rp;
            }
            else {
                printf("0");
                if(np->lp==NULL){
                    Node *child = (Node *)malloc(sizeof(Node));
                    child->rp = NULL;
                    child->lp = NULL;
                    np->lp = child;
                }
                (np->num)++;
                np = np->lp;
            }
        }
        printf("\n");
    } 
    return;
}

/*  build_parallel:  order for array -> order for tree   */
void build_parallel(Node *np, ui *array){
    #pragma omp parallel
    {   
        #pragma omp for
        for(int i=0; i<size;i++){
            int x = array[i];
            for(int k=31;k>=0;k--){
                if(((1<<k)& x)!=0){
                    if(np->rp==NULL){
                        Node *child = (Node *)malloc(sizeof(Node));
                        child->rp = NULL;
                        child->lp = NULL;
                        np->rp = child;
                    }
                    (np->num)++;
                    np = np->rp;
                }
                else {
                    if(np->lp==NULL){
                        Node *child = (Node *)malloc(sizeof(Node));
                        child->rp = NULL;
                        child->lp = NULL;
                        np->lp = child;
                    }
                    (np->num)++;
                    np = np->lp;
                }
            }
        } 
        
    }
    return;
}

/* global variable for dfs  */
ui index;
/*  dfs:    order for tree -> order forarray   */
void dfs(Node *n, ui *array, int k, ui num){
    ui dfsnum=0;   
    bool leafflag = true;
    printf("=================================\n===   dfs(%u, %u)   ===\n", 1 <<(POWMAX -k), num);
    if((n->lp)!=NULL) {
        printf("lp->\n");
        dfs(n->lp, array, k+1, num);
        leafflag = false;
    }
    if((n->rp)!=NULL){
        dfsnum = num + (1<<(POWMAX-k));
        printf("rp->\n");
        dfs(n->rp, array, k+1, dfsnum);
        leafflag = false;
    }
    if(leafflag){
        array[index] = num;
        printf ("array[%u] = %u\n", index, num);
        index++;
    }
    printf("==================================\n");
    return;
} 

void delete(Node *n){
    if(n->lp!=NULL) delete(n->lp);
    if(n->rp!=NULL) delete(n->rp);
    free(n);
    return;
}

void shuffle(ui *array);

double sort1(ui *array){
    double time;

    int i;

    time = omp_get_wtime();
    //逐次ソート開始
    Node *root = (Node *)malloc(sizeof(Node));
    root->num = size;
    root->lp = NULL;
    root->rp = NULL;

    build(root, array);
    printf("=========================\n\t === dfs start ===\t\n===========================\n");
    dfs(root, array, 0, 0);
    delete(root);
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

double sort2(int *array){
    double time;

    int i;

    time = omp_get_wtime();
    //並列ソート開始
    Node *root = (Node *)malloc(sizeof(Node));
    root->num = size;
    root->lp = NULL;
    root->rp = NULL;

    build_parallel(root, array);
    dfs(root, array, 0, 0);
    delete(root);
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
    time = omp_get_wtime();
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
    double time2 = sort2(array2);

    printf("time1 = %lf, time2 = %lf\n", time1, time2);
    printf("speedup = %lf\n", time1 / time2);

    free(array1);
    free(array2);
}