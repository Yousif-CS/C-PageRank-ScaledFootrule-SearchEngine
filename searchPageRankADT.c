//Implementation of searchPageRankADT

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "searchPageRankADT.h"


#define SORT_BY_SW_COUNT 0
#define SORT_BY_PAGE_RANK 1

struct urlList{
    UrlNode * list;
    int nNodes;
    int size;
};

UrlList newurlList(int size){
    UrlList n = malloc(sizeof(*n));
    assert(n != NULL);
    n->list = malloc(sizeof(UrlNode) * size);
    n->size = size;
    n->nNodes = 0;
    assert(n->list != NULL);
    for(int i = 0; i < size; i++){
        UrlNode node; node.url = NULL; node.PR = 0; node.SwCount = 0;
        n->list[i] = node;
    }
    return n;
}

//Adds a url node containing the url string and the pagerank
void urlListAdd(UrlList ul, char * url, double pr){
    if (ul == NULL)
        return;

    if (ul->nNodes == ul->size){
        ul->list = realloc(ul->list, 2 * ul->size * sizeof(UrlNode));
        assert(ul->list != NULL);
        ul->size *= 2;
    }

    UrlNode n;
    n.url = malloc(sizeof(char) * (strlen(url) + 1));
    n.url = strcpy(n.url, url);
    n.SwCount = 0;
    n.PR = pr;
    ul->list[ul->nNodes++] = n;
}

//increments the number of search words occuring in the url
void incrementSwCount(UrlList ul, char * url, int incr){
    if (ul == NULL)
        return;
    
    for (int i = 0; i < ul->nNodes; i++){
        if (strcmp(ul->list[i].url, url) == 0){
            ul->list[i].SwCount += incr;
            return;
        }
    }
    fprintf(stderr, "Could not find url inside UrlList\n");
}


//fetch a node at a certain index
UrlNode urlListNode(UrlList ul, int i){
    if (ul == NULL){
        UrlNode n; n.url = NULL; n.SwCount = 0; n.PR = 0;
        return n;
    }
    return ul->list[i];
}

void merge(UrlNode *l, int lo, int mid, int hi, int option){
    UrlNode tmp[hi -lo + 1];

    int i = lo, j = mid + 1, k = 0;

    while(i <= mid && j <= hi){
        if (option == SORT_BY_PAGE_RANK){
            if (l[i].PR >= l[j].PR)
                tmp[k++] = l[i++];
            else
                tmp[k++] = l[j++];
            
        }else if (option == SORT_BY_SW_COUNT){
            if (l[i].SwCount >= l[j].SwCount)
                tmp[k++] = l[i++];
            else
                tmp[k++] = l[j++];
        }
    }
    //copy the rest
    while(i <= mid) tmp[k++] = l[i++];
    while(j <= hi) tmp[k++] = l[j++];

    //copy the temporary array into our original array
    for(int i = lo, k = 0; i <= hi; i++, k++){
        l[i] = tmp[k];
    }
}

void doMergeSort(UrlNode *l, int lo, int hi, int option){
    if (lo >= hi)
        return;
    
    int mid = (lo + hi) / 2;
    doMergeSort(l, lo, mid, option);
    doMergeSort(l, mid + 1, hi, option);
    merge(l, lo, mid, hi, option);

}

//sorts a segment by the number of search words in each url
void sortBySwCount(UrlList ul, int lo, int hi){
    if (ul == NULL)
        return;
    doMergeSort(ul->list, lo, hi, SORT_BY_SW_COUNT);
}

//sorts a segment by pagerank
void sortByPR(UrlList ul, int lo, int hi){
    if (ul == NULL)
        return;
    doMergeSort(ul->list, lo, hi, SORT_BY_PAGE_RANK);
}

int urlListNumElements(UrlList ul){
    if (ul == NULL)
        return 0;
    return ul->nNodes;
}
//destroy list
void destroyurlList(UrlList ul){
    if (ul == NULL)
        return;

    for(int i = 0; i < ul->nNodes; i++){
        free(ul->list[i].url);
    }
    free(ul->list);
    free(ul);
}

//debugging purposes
void printurlList(UrlList ul){
    if (ul == NULL)
        return;

    for(int i = 0; i < ul->nNodes; i++)
        printf("%s  %d  %0.7lf\n", ul->list[i].url, ul->list[i].SwCount, ul->list[i].PR);
}