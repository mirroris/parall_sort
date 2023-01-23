#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include<stdbool.h>
#include<unistd.h>
#define ui unsigned int 
#define POWMAX 31
#define MOD 1
#define MAXTHRDS 32
#define STKSIZE 33

ui ml;
int measure[32];
ui *tmparray[32];

/*  global for all programs */
int size, seed;
struct Node{
    ui num;
    struct Node *rp;
    struct Node *lp;
};
typedef  struct Node Node;

Node *astk[STKSIZE];
Node *bstk[STKSIZE];

int dfs(Node *n, ui *array, int k, ui num, int index);
void delete(Node *n);
void merge(Node *a, Node *b);

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
void build_parallel(Node **root, ui *array, int size){
    Node *np;
    int Nthrds;
    #pragma omp parallel 
    {
        int id;
        id = omp_get_thread_num();
        Nthrds = omp_get_num_threads();
        #pragma omp barrier
        if(id == 0){
            ml = (1LL<<32) /Nthrds;
            for(int i=0;i<size;i++){
                ui x = array[i];
                ui y = x/ml;
                tmparray[y][measure[y]++]= x;
            }
        }
        /*
        if(id == 0){
            for(int i=0;i<Nthrds;i++){
                printf("tmparray[%d] == \n",i);
                for(int j=0;j<measure[i];j++){
                    printf("%u ",tmparray[i][j]);
                }
                printf("\n");
            }
        }*/
        #pragma omp barrier
            if(measure[id]!=0){
            build(root[id], tmparray[id], measure[id]);
            int lindex = 0;
            for(int i=0;i<id;i++) lindex+=measure[i];
            //printf("index of id(%d) = %d\n",id,lindex);
            dfs(root[id], array, 0, 0, lindex);
        }
        delete(root[id]);
    }
    /*
        if(id<Nthrds-1) {
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
        /*
        int d = Nthrds;
        int prevd = Nthrds;
        int preprevd = Nthrds;
        while(d>1){
        d = 1 + ((d - 1)/2); 
        if((id+d) < prevd) {
            printf("merge(%d, %d)\n", id, id+d);
            merge(root[id], root[id+d]);
        }
        else if((prevd<=id) && (id<preprevd)){
            printf("delete(%d)\n", id);
            delete(root[id]);
        }
        prevd = d;
        preprevd = prevd;
        #pragma omp barrier
        }
        */
    
    /*
    printf("root[0]=\n");
    dfs(root[0], array, 0, 0);
    printf("root[1]=\n");
    dfs(root[1], array, 0, 0);
    merge(root[0], root[1]);  
    printf("root[0]=\n");
    dfs(root[0], array, 0, 0);
    */
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
            if(anp->lp!=NULL) {
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
            if(anp->rp!=NULL) {
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

/*
void merge(ui *array1, ui *array2, int begin, int mid, int end){
    int i=begin,j=end;
    ui tmp=0;
    while((i<end) && (j<end)){
        tmp = array2[j];
        while((i<j) && array1[i]=<tmp)i++;
        tmp = array1[i];
        while((j<end) && tmp>array2[j])j++;
        tmp = array1[i];
        array[i] = array[j];
        array[j] = tmp;
    }
    while(i<mid) array1[i] = array2[j];
    while(j<end) array1[i] = array2[]
    return;
}*/

void merge(Node *a, Node *b){
    bool nullflag = true;
    if(b->lp!=NULL){
        nullflag = false;
        if(a->lp!=NULL) merge(a->lp, b->lp);
        else a->lp = b->lp;
    }
    if(b->rp!=NULL){
        nullflag = false;
        if(a->rp!=NULL) merge(a->rp, b->rp);
        else a->rp = b->rp;
    }
    if(nullflag){
        a->num+=b->num;
    }
    return;
}

/*  dfs:    order for tree -> order forarray   */
int dfs(Node *n, ui *array, int k, ui num, int index){
    ui dfsnum=0;   
    bool leafflag = true;
    int tindex=index;
    if(n->lp!=NULL) {
        //printf("dfs(%u)\n", n->num);
        tindex = dfs(n->lp, array, k+1, num, tindex);
        leafflag = false;
    }
    if(n->rp!=NULL){
        dfsnum = num + (1<<(POWMAX-k));
        //printf("dfs(%u)\n", n->num);
        tindex = dfs(n->rp, array, k+1, dfsnum, tindex);
        leafflag = false;
    }
    if(leafflag){
        ui cnt = n->num;
        while(cnt-->0){
            //printf("array[%d] = %u\n", tindex, num);
            array[tindex] = num;
            tindex++;
        }
    }
    return tindex;
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

    build(root, array, size);
    printf ("build (%lf)\n", omp_get_wtime()-time);
    dfs(root, array, 0, 0,0);
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
    for(int i=0;i<4;i++)measure[i] = 0;
    for(int i=0;i<4;i++)tmparray[i] = (ui *)malloc(sizeof(ui)*size);
    Node **root = (Node **)malloc(sizeof(Node *)*MAXTHRDS);
    for(int i=0;i<4;i++){
        root[i] = (Node *)malloc(sizeof(Node));
        root[i]->rp = NULL;
        root[i]->lp = NULL;
    }
    time = omp_get_wtime();
    //並列ソート開始
    build_parallel(root, array, size);
    printf ("build_parallel (%lf)\n", omp_get_wtime()-time);
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
    for(int i=0;i<4;i++)free(tmparray[i]);
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
        array1[i] = rand();
        array2[i] = array1[i];
    }

    double time1 = sort1(array1);
    double time2 = sort2(array2);

    printf("time1 = %lf, time2 = %lf\n", time1, time2);
    printf("speedup = %lf\n", time1 / time2);

    free(array1);
    free(array2);
}