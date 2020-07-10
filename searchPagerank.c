//Implementation of a search engine;
//ranks urls in terms of the descending number of search words, then by using
//the page rank

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "searchPageRankADT.h"

#define MAX_LEN 100
#define MAX_LINE 1000
#define INITIAL_SIZE 10

char ** tokenize(char *, char *);
void freeTokens(char **);
void printUrls(UrlList);
void replaceChar(char *, char, char);
char * myStrdup(char *);


void fillUrlList(UrlList, char *);
void subSortByPR(UrlList);

int main(int argc, char *argv[]){

    if (argc < 3){
        fprintf(stderr, "Usage: %s [search words]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    //prepare the list of urls and their pagerank
    UrlList ul = newurlList(INITIAL_SIZE);

    //fill in the urllist with urls and their pageranks
    fillUrlList(ul, "pagerankList.txt");

    //read in the searchWords and tokenise them
    char line[MAX_LINE];
    FILE * swfp = fopen("invertedIndex.txt", "r");
    if (swfp == NULL){
        fprintf(stderr, "Cannot open Inverted Index File");
        destroyurlList(ul);
        exit(EXIT_FAILURE);
    }

    while(fgets(line, MAX_LINE, swfp) != NULL){
        line[strlen(line) -1] = '\0'; //stripping newline
        char ** tokens = tokenize(line, " ");
        int i = 1;
        //skip lines that are not relevant
        for(; i < argc; i++){
            if (strcmp(tokens[0], argv[i]) == 0)
                break;
        }
        if (i == argc){
            freeTokens(tokens);
            continue;
        }
        // for every url inside the tokens, increment the search word count by 1
        for(int i = 1; tokens[i] != NULL; i++)
            incrementSwCount(ul, tokens[i], 1);

        freeTokens(tokens);
    }
    //closing the search word file
    fclose(swfp);

    // generally sort the list by search word count
    sortBySwCount(ul, 0, urlListNumElements(ul) - 1);
    
    // sort the subsequent groups with the same search word count by PR
    // this is not necessary if the pagerankList.txt file has them in order
    // as the sorting algorithm for sw count is stable (merge sort)
    subSortByPR(ul);
    printUrls(ul);
    destroyurlList(ul);

    return 0;
}


void fillUrlList(UrlList ul, char * filename){
    FILE * prfp = fopen(filename, "r");
    if (prfp == NULL){
        fprintf(stderr, "Cannot open %s\n", filename);
        exit(EXIT_FAILURE);
    } 

    double pr = -1;
    char url[MAX_LEN];
    char format[MAX_LINE];
    int degree = -1; // we do not need this so we will ignore it
    while(fgets(format, MAX_LINE, prfp) != NULL){
        replaceChar(format, ',', ' '); //replace all commas with spaces
        if (sscanf(format, "%s %d %lf", url, &degree, &pr) != 3)
            continue;
        urlListAdd(ul, url, pr);
    }
    fclose(prfp);
}

void subSortByPR(UrlList ul){
    // for each group of equal search word count, sort by page rank;
    int start = 0;
    int end = 0;
    for(int i = 0; i < urlListNumElements(ul); i++){
        if (i != 0){
            UrlNode nb  = urlListNode(ul, i - 1); //at an index;
            UrlNode nc = urlListNode(ul, i);
            if (nb.SwCount != nc.SwCount){
                end = i - 1;
                sortByPR(ul, start, end);
                start = i;
                end = i;
            }
        }   
    }
    //sorting the last group
    sortByPR(ul, start, urlListNumElements(ul) - 1);
}
//returns a null terminated array of tokens
char **tokenize(char *s, char *sep){

    char *token;
    char **tokens;

    //temp copy of string
    //figure out the size of the 2d array
    char *tmp = myStrdup(s);
    int n = 0;
    strtok(tmp, sep); n++;
    while(strtok(NULL, sep) != NULL) n++;
    free(tmp);

    tokens = malloc(sizeof(char*) * (n+1));
    token = strtok(s, sep);
    int i = 0;
    tokens[i++] = myStrdup(token);
    while((token = strtok(NULL, sep)) != NULL){
        tokens[i++] = myStrdup(token);
    }
    tokens[i] = NULL;

    return tokens;
}

//frees the array of tokens
void freeTokens(char ** t){
    if (t == NULL)
        return;

    for(int i = 0; t[i] != NULL; i++){
        free(t[i]);
    }
    free(t);
}

//print the urls in the correct format
void printUrls(UrlList ul){
    if (ul == NULL)
        return;

    for(int i = 0; i < urlListNumElements(ul); i++){
        if (i >= 30)
            break;
        UrlNode n = urlListNode(ul, i);
        printf("%s\n", n.url);
    }
}

void replaceChar(char * s, char old, char new){
    for(int i = 0; s[i] != '\0'; i++)
        if(s[i] == old)
            s[i] = new;
}

char * myStrdup(char *s){
    if (s == NULL)
        return NULL;
    
    char * new = malloc(sizeof(char) * (strlen(s) + 1));
    assert(new != NULL);
    new = strcpy(new, s);
    return new;
}

