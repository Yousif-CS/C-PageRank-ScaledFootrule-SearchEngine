//implementing priority queue

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "PQueue.h"

#define greater(a, b) a.dist > b.dist

//mainly sourced from the lecture slides
PQueue newPQueue(int size){
    PQueue n = malloc(sizeof(*n));
    assert(n != NULL);
    n->h = malloc(sizeof(UrlNode) * (size + 1));
    assert(n->h != NULL);
    n->size = size;
    n->nElements = 0;

    return n;
}

static void swap(UrlNode h[], int i, int j){
    UrlNode tmp = h[i];
    h[i] = h[j];
    h[j] = tmp;
}
static void fixDown(UrlNode h[], int i, int N){
    while(2 * i <= N){  
        int j = 2 * i;

        //choose smaller of two children
        if (j < N && greater(h[j], h[j+1])) j++;
        if (!(greater(h[i], h[j]))) break;
        swap(h, i, j);
        i = j;
    }
}

static void fixUp(UrlNode h[], int i){
    while(i > 1 && greater(h[i/2], h[i])){
        swap(h, i, i/2);
        i = i / 2;
    }
}
void PQueuePush(PQueue pq, UrlNode n){
    if (pq->nElements == pq->size){
        pq->size *= 2;
        pq->h = realloc(pq->h, pq->size);
    };
    pq->h[++pq->nElements] = n;
    fixUp(pq->h, pq->nElements);
}

UrlNode PQueuePop(PQueue pq){
    UrlNode top = pq->h[1];
    //overwrite first by last
    pq->h[1] = pq->h[pq->nElements];
    pq->nElements--;
    //move new root to correct position
    fixDown(pq->h, 1, pq->nElements);
    return top;
}

void PQueueDestroy(PQueue pq){
    if (pq == NULL)
        return;

    free(pq->h);
    free(pq);
}

int PQueueEmpty(PQueue pq){
    if (pq == NULL)
        return 1;
    return pq->nElements == 0;
}


void printPQueue(PQueue q){

    int i = 1;
    while(2 * i <= q->nElements){
        printf("%lf\n", q->h[i].dist);
        printf("%lf %lf\n", q->h[2*i].dist, q->h[2*i + 1].dist);
        i = 2 * i;
    }
}