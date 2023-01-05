#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include<stdbool.h>
#include<unistd.h>
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
void build(Node *root, ui *array){
    Node *np; 
    for(int i=0; i<size;i++){ 
        np = root;
        ui x = array[i];
        //printf("x = %u\n",x);
        for(int k=31;k>=0;k--){
            if(((1<<k)& x)!=0){
                if((np->rp)==NULL){
                    Node *child = (Node *)malloc(sizeof(Node));
                    child->rp = NULL;
                    child->lp = NULL;
                    child->num = 0;
                    np->rp = child;
                    //printf("(%u)",(1 << k));
                }
                np = np->rp;
                (np->num)++;
                //printf("->");
            }
            else {
                if((np->lp)==NULL){
                    Node *child = (Node *)malloc(sizeof(Node));
                    child->rp = NULL;
                    child->lp = NULL;
                    child->num = 0;
                    np->lp = child;
                    //printf("(%u)",(1 << k));
                }
                np = np->lp;
                (np->num)++;
                //printf("->");
            }
            //printf("\n");
        }
    } 
    return;
}

/*  build_parallel:  order for array -> order for tree   */
void build_parallel(Node **root, ui *array){
    Node *np;
    int Nthrds;
    #pragma omp parallel 
    {
        int id;
        id = omp_get_thread_num();
        Nthrds = omp_get_num_threads();
        root[id] = (Node *)malloc(sizeof(Node));
        root[id]->num = 0;
        root[id]->rp = NULL;
        root[id]->lp = NULL;

        for(int i=id; i<size;i+=Nthrds){ 
            np = root[id];
            ui x = array[i];
            //printf("x = %u\n",x);
            for(int k=31;k>=0;k--){
                if(((1<<k)& x)!=0){
                    if((np->rp)==NULL){
                        Node *child = (Node *)malloc(sizeof(Node));
                        child->rp = NULL;
                        child->lp = NULL;
                        child->num = 0;
                        np->rp = child;
                        //printf("(%u)",(1 << k));
                    }
                    np = np->rp;
                    (np->num)++;
                    //printf("->");
                }
                else {
                    if((np->lp)==NULL){
                        Node *child = (Node *)malloc(sizeof(Node));
                        child->rp = NULL;
                        child->lp = NULL;
                        child->num = 0;
                        np->lp = child;
                        //printf("(%u)",(1 << k));
                    }
                    np = np->lp;
                    (np->num)++;
                    //printf("->");
                }
                //printf("\n");
            }
        } 
#pragma omp barrier
    // ここでrootをマージしたい
    // while (k>1) root =  merge(root, k) (k分割マージ)
    }

    int sum=0;
    for(int i=0;i<(Nthrds-1);i++) {
    }
    return;
}

void full_merge(ui *array, int a,int b, int c){
    int l=a,r=b;
    while(array[l] > array[r]) l++;

    return;
} 

void merge(Node *a, Node *b, int k){
    //F-Treeのままマージする場合との違い
    //mergedfs(a);

    return;
}

ui index;
/*  dfs:    order for tree -> order forarray   */
void dfs(Node *n, ui *array, int k, ui num){
    ui dfsnum=0;   
    bool leafflag = true;
    if(n->lp!=NULL) {
        //printf("dfs(%u)\n", n->num);
        dfs(n->lp, array, k+1, num);
        leafflag = false;
    }
    if(n->rp!=NULL){
        dfsnum = num + (1<<(POWMAX-k));
        //printf("dfs(%u)\n", n->num);
        dfs(n->rp, array, k+1, dfsnum);
        leafflag = false;
    }
    if(leafflag){
        ui cnt = n->num;
        //printf("cnt = %u\n", cnt);
        while(cnt-->0){
            array[index] = num;
            index++;
        }
    }
    return;
} 

/*  dfs_parallel:    order for tree -> order forarray   */
void dfs_parallel(Node *n, ui *array, int k, ui num){
    ui dfsnum=0;   
    bool leafflag = true;
    if(n->lp!=NULL) {
        dfs(n->lp, array, k+1, num);
        leafflag = false;
    }
    if(n->rp!=NULL){
        dfsnum = num + (1<<(POWMAX-k));
        dfs(n->rp, array, k+1, dfsnum);
        leafflag = false;
    }
    if(leafflag){
        ui cnt = n->num;
        while(cnt-->0){
            array[index] = num;
            index++;
        }
    }
    return;
} 

void delete(Node *n){
    if(n->lp!=NULL) delete(n->lp);
    if(n->rp!=NULL) delete(n->rp);
    free(n);
    return;
}

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
    printf ("build (%lf)\n", omp_get_wtime()-time);
    index = 0;
    dfs(root, array, 0, 0);
    printf ("dfs (%lf)\n", omp_get_wtime()-time);
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

double sort2(ui *array){
    double time;

    int i;

    time = omp_get_wtime();
    //並列ソート開始
    Node **root = (Node **)malloc(sizeof(Node *));
    build_parallel(root, array);
    printf ("build_parallel (%lf)\n", omp_get_wtime()-time);
    index = 0;
    dfs(root[0], array, 0, 0);
    printf ("dfs (%lf)\n", omp_get_wtime()-time);
    delete(root[0]);
    free(root);
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