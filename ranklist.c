#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "ranklist.h"


#define MAX_LEN 100

static char * myStrdup(char *s){
    if (s == NULL)
        return NULL;
    
    char * new = malloc(sizeof(char) * (strlen(s) + 1));
    assert(new != NULL);
    new = strcpy(new, s);
    return new;
} 

List newRankList(int size){
    
    List new = malloc(sizeof(*new));
    assert(new != NULL);

    //adding 1 because it is one based 
    new->l = malloc(sizeof(char *) * (size + 1));
    assert(new->l != NULL);

    for(int i = 1; i <= size; i++){
        new->l[i] = NULL;
    }

    new->size = size;
    new->nElements = 0;
    return new;
}

void destroyRankList(List rl){
    if (rl == NULL)
        return;
    
    for(int i = 1; i <= rl->nElements; i++){
        free(rl->l[i]);
    }
    free(rl->l);
    free(rl);
}

//no duplicates
void RankListAdd(List rl, char *url){
    if (rl == NULL)
        return;
    for(int i = 1; i <= rl->nElements; i++){
        //string already exists; just return
        if (strcmp(rl->l[i], url) == 0)
            return;
    }
    if (rl->nElements == rl->size){
        rl->size *= 2;
        rl->l = realloc(rl->l, rl->size);
    }
    rl->l[++rl->nElements] = myStrdup(url);
}

//list fetch index of char
int RankListFetch(List rl, char*url){
    if (rl == NULL)
        return -1;
    
    for(int i = 1; i <= rl->nElements; i++){
        if (strcmp(rl->l[i], url) == 0)
            return i;
    }
    //does not exist
    return -1;
}

void RankListRead(List rl, char *filename){
   
    FILE *fp = fopen(filename, "r");
    if (fp == NULL){
        fprintf(stderr,"Cannot open %s\n", filename);
        return;
    }
    char s[MAX_LEN];
    while(fscanf(fp, "%s\n", s) == 1){
        RankListAdd(rl, s);
    }
    fclose(fp);
}

List UnionList(List ls[], int size){
    if (ls == NULL)
        return NULL;
    
    List n = newRankList(ls[0]->size);

    for(int i = 0; i < size; i++){
        if (ls[i] == NULL)
            continue;
        for(int j = 1; j <= ls[i]->nElements; j++){
            RankListAdd(n, ls[i]->l[j]);
        }
    }

    return n;
}


void printRankList(List rl){
    if (rl == NULL){
        fprintf(stderr, "list empty!\n");
        return;
    }
    for(int i = 1; i <= rl->nElements; i++){
        printf("%s  ", rl->l[i]);
    }
    printf("\n");
}