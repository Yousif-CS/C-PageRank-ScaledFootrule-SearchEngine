//implementing the PageRank ADT

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "PageRankADT.h"

struct _pageRankNode{
    double prVal;
    Vertex vertex;
};

struct pageRank{
    PageRankNode * list;
    int nElements;
    int size;
};
//create a new PageRank list
PageRank newPageRank(int size){
    /*
    Create a new pagerank list with initial size
    */
    PageRank pr = malloc(sizeof(struct pageRank));
    assert (pr != NULL);
    pr->list = malloc(sizeof(PageRankNode) * size);
    assert (pr->list != NULL);
    pr->size = size;
    pr->nElements = 0;
    return pr;
}

void PageRankInsert(PageRank pr, double prVal, Vertex v){
    /*
    Inserts a struct of pagerank and node into the list
    adjusts size when necessary
    */
    PageRankNode n; n.prVal = prVal; n.vertex = v;
    if (pr->nElements == pr->size){
        pr->list = realloc(pr->list, 20 * pr->size);
        pr->size *= 2;
    }
    pr->list[pr->nElements++] = n;


}

double PageRankValue(PageRank pr, Vertex v){
    /*
    fetches the pagerank of a certain vertex
    */
    if (pr == NULL)
        return NOT_FOUND;
    for (int i = 0; i < pr->nElements; i++)
        if (pr->list[i].vertex == v)
            return pr->list[i].prVal;

    return NOT_FOUND;
}

void PageRankUpdate(PageRank pr, double newPrVal, Vertex v){
    /*
    Update the pagerank of a certain page
    */
    
    if (pr == NULL)
        return;

    for(int i = 0; i < pr->nElements; i++){
        if (pr->list[i].vertex == v){
            pr->list[i].prVal = newPrVal;
            return;
        }
    }
}
int PageRankSize(PageRank pr){
    if (pr == NULL)
        return -1;

    return pr->nElements;
}
void destroyPageRank(PageRank pr){
    if (pr == NULL)
        return;

    free(pr->list);
    free(pr);
}

static void merge(PageRankNode *a, int lo, int mid, int hi){
    PageRankNode tmp[hi - lo + 1];
    int i = lo, j = mid + 1, k = 0;

    while (i <= mid && j <= hi){
        if (a[i].prVal >= a[j].prVal)
            tmp[k++] = a[i++];
        else
            tmp[k++] = a[j++];
    }
    //copying the rest of the elements
    while (i <= mid) tmp[k++] = a[i++];
    while(j <= hi) tmp[k++] = a[j++];

    //copying to the original
    for(int i = lo, k = 0; i <= hi; i++, k++)
        a[i] = tmp[k]; 
}
static void doMergeSort(PageRankNode *a, int lo, int hi){
    if (lo >= hi)
        return;
    int mid = (lo + hi) / 2;
    doMergeSort(a, lo, mid);
    doMergeSort(a, mid + 1, hi);
    merge(a, lo, mid, hi);
}
void PageRankSort(PageRank pr){
    if (pr == NULL)
        return;

    doMergeSort(pr->list, 0, pr->nElements-1);
}

void outputPageRank(FILE *fp, PageRank pr, Graph g, LookupTable t){

    for (int i = 0; i < PageRankSize(pr); i++){
        Vertex v = pr->list[i].vertex;
        double prVal = pr->list[i].prVal;
        fprintf(fp, "%s, %d, %.7f\n", tableFetchKey(t, v), degree(g, v), prVal);
    }
}


//debugging purposes
void printPageRank(PageRank pr){
    if (pr == NULL)
        return;

    for(int i = 0; i < pr->nElements; i++)
        printf("%d ->   %f\n", pr->list[i].vertex, pr->list[i].prVal);
}