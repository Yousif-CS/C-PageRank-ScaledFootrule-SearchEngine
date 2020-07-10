#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#include "PQueue.h"
#include "ranklist.h"

#define MAX_LISTS 10
#define DEFAULT_SIZE 10
#define SECOND_SMALLEST 0
#define SMALLEST 1

//calculate the foot rule distance of
//a certain url based on the position p
//returns the distance
double w(char *c, int p, int k);

//creates a matrix of size 'size'
double ** createMatrix(int size);

//destroy matrix
void destroyMatrix(double ** m, int size);

//
double ** CreateDistanceMatrix(int P[], int nLists);
double bestPermutation(double ** m, int P[], int size);
double ** calculateDist(double **m, int size);
void printBestPermutation(int P[], double dist);

List lists[MAX_LISTS];
List merged;

int main(int argc, char *argv[]){
    if (argc < 2){
        fprintf(stderr, "Usage: %s [rank list files]\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    for(int i = 1; i < argc; i++){
        lists[i-1] = newRankList(DEFAULT_SIZE);
        RankListRead(lists[i-1], argv[i]);
    }

    merged = UnionList(lists, argc - 1);
    int P[merged->nElements];
    for(int i = 0; i < merged->nElements; i++){
        P[i] = i + 1;
    }

    double **m = CreateDistanceMatrix(P, argc -1);

    //calculateDist(m, merged->nElements);
    double dist = bestPermutation(m, P, merged->nElements);
    printBestPermutation(P, dist);

    destroyMatrix(m,merged->nElements);

    destroyRankList(merged);
    for(int i = 0; i < argc - 1; i++){
        destroyRankList(lists[i]);
    }

    return 0;
}

double bestPermutation(double ** m, int P[], int size){
    
    PQueue q = newPQueue(size * size);
    int visitedPos[size + 1];
    int visitedUrl[size + 1];
    memset(visitedPos, 0, sizeof(int) * (size + 1));
    memset(visitedUrl, 0, sizeof(int) * (size + 1));

    for(int i = 1; i <= size; i++){
        for(int j = 1; j <= size; j++){
            UrlNode n;
            n.dist = m[i][j];
            n.speculatedPos = j;
            n.url = i;
            PQueuePush(q, n);
        }
    }
    double totalDist = 0;
    while(!PQueueEmpty(q)){
        UrlNode n = PQueuePop(q);
        if (visitedPos[n.speculatedPos] || visitedUrl[n.url])
            continue;
        
        totalDist += n.dist;
        P[n.url - 1] = n.speculatedPos;
        visitedPos[n.speculatedPos] = 1;
        visitedUrl[n.url] = 1;
    }
    PQueueDestroy(q);
    return totalDist;
}

double ** CreateDistanceMatrix(int P[], int nLists){

    double ** m = createMatrix(merged->nElements);

    for(int i = 1; i <= merged->nElements; i++){
        for(int j = 0; j < merged->nElements; j++){
            m[i][P[j]] = w(merged->l[i], P[j], nLists);
        }
    }
    return m;
}

void printBestPermutation(int P[], double diff){
    printf("%lf\n", diff);
    for(int i = 0; i < merged->nElements; i++){
        printf("%s\n", merged->l[P[i]]);
    }
}
//k is the number of the lists
double w(char *c, int p, int k){

    double sum = 0;
    for(int i = 0; i < k; i++){
        int pos = RankListFetch(lists[i], c);
        if (pos < 0)    //error does not exist; continue;
            continue;
        sum += fabs(1.0 * pos/lists[i]->nElements - 1.0 * p/merged->nElements); 
    }
    return sum;
}

double ** createMatrix(int size){
    //we add 1 to the malloc because it is one based and not zero based
    double ** m = malloc(sizeof(double *) * (size + 1));
    assert(m != NULL);

    for(int i = 1; i <= merged->nElements; i++){
        // it is a one based matrix; i.e. ignore m[0][j] and m[i][0]
        m[i] = malloc(sizeof(double) * (size + 1));
        assert(m[i] != NULL);
    }
    return m;

}

void destroyMatrix(double ** m, int size){
    for(int i = 1; i <= size; i++){
        free(m[i]);
    }
    free(m);
}