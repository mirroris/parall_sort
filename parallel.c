#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include<stdbool.h>
#include<unistd.h>
#define ui unsigned int 
#define POWMAX 31
#define MOD 1024
#define MAXTHRDS 32
#define STKSIZE 33


/*  global for all programs */
int size, seed;
int Nthrds;
struct Node{
    ui num;
    ui val;
    struct Node *lp;
    struct Node *rp;
};
typedef  struct Node Node;

Node *astk[STKSIZE];
Node *bstk[STKSIZE];

int chunk[MAXTHRDS];

ui index;
void dfs(Node *n, ui *array, int k, ui num);
void delete(Node *n);
void merge(Node *a, Node *b);
void fullmerge(ui *array, int *chunk);

/*  build:  order for array -> order for tree   */
void build(Node *root, ui *array, int size){
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
                    /*if(k!=0) child->leaf = false;
                    else child->leaf = true;
                    */
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
                    /*if(k!=0) child->leaf = false;
                    else child->leaf = true;
                    */np->lp = child;
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

void build(Node *np, ui *array, int k, ui x){
    bool leaf = true;
    if((x&(1<<k))==0){
        /* 左の部分木のどこに挿入するか */
        if(np->lp!=NULL) {
            if(np->lp->val<=x)uild(np->lp, array, k+1, x);
            else 
        }
        else {
            Node *child = (Node *)malloc(sizeof(Node));
            child->lp = NULL;
            child->rp = NULL;
            child->num = 0;
            child->val = x;
        }
    }
    else {
        if(np->rp!=NULL) build(np->rp, array, k+1, x);
    }
    if(leaf)
}

/*  build_parallel:  order for array -> order for tree   */
void build_parallel(Node **root, ui *array, int size){
    Node *np;
    #pragma omp parallel 
    {
        int id;
        id = omp_get_thread_num();
        Nthrds = omp_get_num_threads();
        int chunk = 1+(size-1)/Nthrds;
        int rent = size%Nthrds;
        if(id ==0)printf("chunk = %d, rent = %d\n",chunk, rent);
        root[id] = (Node *)malloc(sizeof(Node));
        root[id]->rp = NULL;
        root[id]->lp = NULL;
        if(id!=(Nthrds-1)) {
            root[id]->num = chunk;
            build(root[id], array+id*chunk, chunk);
        }
        else {
            root[id]->num = rent;
            build(root[id], array+id*chunk, rent);
        }
    #pragma omp barrier
    // ここでrootをマージしたい
    // while (k>1) root =  merge(root, k) (k分割マージ)
    
        int d = Nthrds;
        int prevd = Nthrds;
        int preprevd = Nthrds;
        /*
        while(d>1){
        d = 1 + ((d - 1)/2); 
        if((id+d) < prevd) {
            //printf("merge(%d, %d)\n", id, id+d);
            //dfs(root[id], array, 0, 0);
            //dfs(root[id+d], array, 0, 0);
            merge(root[id], root[id+d]);
        }
        prevd = d;
        preprevd = prevd;
        #pragma omp barrier
        }
        */
       
    /*
    int d =Nthrds;
    int prevd = Nthrds;
    while(d>1){
    d = 1 + ((d - 1)/2); 
    for(int id=0;id<Nthrds;id++){
    if((id+d) < prevd) {
        //printf("merge(%d, %d)\n", id, id+d);
        //dfs(root[id], array, 0, 0);
        //dfs(root[id+d], array, 0, 0);
        merge(root[id], root[id+d]);
        printf("merged root[%d] = \n", id);
        index = 0;
        dfs(root[id], array, 0, 0);
    }
    }
    prevd = d;
    }
    
    printf("root[0]=\n");
    dfs(root[0], array, 0, 0);
    printf("root[1]=\n");
    dfs(root[1], array, 0, 0);
    merge(root[0], root[1]);  
    printf("root[0]=\n");
    dfs(root[0], array, 0, 0);
    */
    }   
    index = 0;
    for(int i=1;i<Nthrds;i++){
        chunk[i]+=chunk[i-1];
    }
    for(int id=0;id<Nthrds;id++){
        index = chunk[id];
        dfs(root[id], array, 0, 0);
    } 
    fullmerge(array ,chunk);
    return;
}

void fullmerge(ui *array, int *chunk){
    for(int i=0;i<Nthrds;i++){
        while(1);
    }
    return;
}

/*
void merge(Node *a, Node *b){
    //F-Treeのままマージする場合との違い
    //対象の構造なのでもしかするとよりよいアルゴリズムが見つかるかもしれない
    //
    Node *anp = a;
    Node *bnp = b;
    int asp=0, bsp = 0;
    astk[asp] = anp;
    bstk[bsp] = bnp;
    bool nullflag;
    do{
        nullflag = true;
        printf("bsp = %d\n",bsp);
        anp = astk[asp];
        bnp = bstk[bsp];

        if(bnp->lp!=NULL){
            bnp = bnp->lp;
            bstk[++bsp] = bnp;
            /*if(anp->lp!=NULL) {
                anp = anp->lp;
                astk[++asp] = anp;
            }
            else {
                // anpにbの葉を追加する。できるだけ最小限のコストに抑える
                anp->lp = bnp;
                bnp = NULL;
            }
        }
        else if(bnp->rp!=NULL){
            bnp = bnp->rp;
            bstk[++bsp] = bnp;
            /*if(anp->rp!=NULL) {
                anp = anp->rp;
                astk[++asp] = anp;
            }
            else {
                // anpにbの葉を追加する。できるだけ最小限のコストに抑える
                anp->rp = bnp;
                bnp = NULL;
            }
        } 
        else {
            if(bsp == STKSIZE) anp->num+=bnp->num;
            bnp = NULL;
            free(bnp);
            asp--,bsp--;
        }
    }while(bsp>0);

    return;
}*/

void merge(Node *a, Node *b){
    bool leafflag = true;
    if(b->lp!=NULL){
        leafflag = false;
        if(a->lp!=NULL) merge(a->lp, b->lp);
        else {
            a->lp = b->lp;
            b->lp = NULL;
        }
    }
    if(b->rp!=NULL){
        leafflag = false;
        if(a->rp!=NULL) merge(a->rp, b->rp);
        else {
            a->rp = b->rp;
            b->rp = NULL;
        }
    }
    if(leafflag){
        a->num+=b->num;
    }
    return;
}

/*  dfs:    order for tree -> order forarray   */
void dfs(Node *n, ui *array, int k, ui num){
    ui dfsnum=0;   
    bool leafflag = true;
    if(n->lp!=NULL) {
        leafflag = false;
        //printf("dfs(%u)\n", n->num);
        dfs(n->lp, array, k+1, num);
    }
    if(n->rp!=NULL){
        leafflag = false;
        dfsnum = num + (1<<(POWMAX-k));
        //printf("dfs(%u)\n", n->num);
        dfs(n->rp, array, k+1, dfsnum);
    }
    if(leafflag){
        ui cnt = n->num;
        //printf("num = %u\n", num);
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
    n=NULL;
    free(n);
    return;
}

void delete_parallel(Node **root){
    #pragma omp parallel
    {
        #pragma omp for
        for(int i=0;i<Nthrds;i++)delete(root[i]);
    }
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

    build(root, array, size);
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
    Node **root = (Node **)malloc(sizeof(Node *)*MAXTHRDS);
    build_parallel(root, array, size);
    //printf ("build_parallel (%lf)\n", omp_get_wtime()-time);
    index = 0;
    dfs(root[0], array, 0, 0);
    //printf ("dfs (%lf)\n", omp_get_wtime()-time);
    delete_parallel(root);
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
        //printf("please input size and seed\n");
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
    //double time2 = sort2(array2);
    double time2 = 0;

    printf("time1 = %lf, time2 = %lf\n", time1, time2);
    printf("speedup = %lf\n", time1 / time2);

    free(array1);
    free(array2);
}