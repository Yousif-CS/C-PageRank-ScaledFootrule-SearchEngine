#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "readData.h"


#define MAX_LENGTH 100


typedef struct node * Node;

struct node {
    char *url;
    Node next;
};
struct urlList {
    Node head;
    Node tail;
    int size;
};
UrlList newUrlList(void){
    /*
    create a new UrlList
    */
    UrlList new = malloc(sizeof(*new));
    assert(new != NULL);
    new->head = new->tail = NULL;
    new->size = 0;
    return new;
}

static Node newUrlNode(char * urlname){
    /*
    create a new list node containing the url file
    name that contains links to other urls
    */
    Node new = malloc(sizeof(*new));
    assert(new != NULL);

    new->url = malloc((strlen(urlname) + 1) * sizeof(char));
    assert(new->url != NULL);

    new->url = strcpy(new->url, urlname);
    new->next = NULL;
    return new;
}
static UrlList insertToList(UrlList urllist, char * url){
    /*
    inserts a node to the tail
    */
    Node new = newUrlNode(url);
    //first time inserting, both head and tail
    //point to the node
    if (urllist->size == 0)
        urllist->head = urllist->tail = new;
    //insert to the tail
    urllist->tail->next = new;
    urllist->tail = new;
    urllist->size++;
    return urllist;
}
UrlList readList(char * collection){
    /*
    Opens a collection file containing all the url files
    which contain links
    
    Always appends to the head -> O(1)
    */
    FILE *fp = fopen(collection, "r");
    assert (fp != NULL);
    char url[MAX_LENGTH];

    UrlList urllist = newUrlList();
    
    while (fscanf(fp, "%s", url) == 1)
        urllist = insertToList(urllist, url);
    
    fclose(fp);
    return urllist;
}

int UrlListSize(UrlList l){
    if (l == NULL)
        return 0;

    return l->size;
}
static void doDestroyUrlList(Node n){
    if (n == NULL)
        return;
    doDestroyUrlList(n->next);
    free(n->url);
    free(n);
}
void destroyUrlList(UrlList urllist){
    /*
    delete all memory associated with urrllist
    */
    if (urllist == NULL)
        return;
    doDestroyUrlList(urllist->head);
    free(urllist);
}

Graph getUrlGraph(UrlList urllist, LookupTable t){
    /*
    goes through a list of urls, opening its file
    and creating a graph based on the links
    */
    Node curr = urllist->head;
    Graph g = newGraph(tableSize(t));
    //traverse list and create graph
    //need to store strings in a lookup table
    while (curr != NULL){
        char toOpen[MAX_LENGTH] = {0};
        sprintf(toOpen, "%s.txt",curr->url);
        FILE * fp = fopen(toOpen, "r");
        assert(fp != NULL);
        //source node
        Vertex v = tableInsert(t, curr->url);
        //start reading "#start Section-1"
        char urllink[MAX_LENGTH];
        fgets(urllink, MAX_LENGTH, fp);
        //basic error check
        if (strcmp(urllink, "#start Section-1\n") != 0){
            fprintf(stderr, "Invalid file format\n");
            exit(EXIT_FAILURE);
        }
        while(fscanf(fp, "%s", urllink) == 1 && strcmp(urllink, "#end")){
            //adjacent nodes
            Vertex w = tableInsert(t, urllink);
            //for now the graph is unweighted
            Edge e; e.v = v; e.w = w; e.weight = -1;
            insertEdge(g, e);
        }
        fclose(fp);
        curr = curr->next;
    }
    //update the weights based on 
    //incoming/outgoing links
    return g;
}
//for debugging purposes
void showUrlGraph(Graph g, LookupTable t){
    /*
    Prints the graph src -> dest format
    */
    assert (g != NULL && t != NULL);
    for (int i = 0; i < graphSize(g); i++){
        Item src = tableFetchKey(t, i);
        if (src == NOITEM)
            continue;
        for (int j = 0; j < graphSize(g); j++){
            if (!edgeExists(g, i, j))
                continue;
            Item dest = tableFetchKey(t, j);
            printf("%s (%d) ->    %s  (%d)  | %f\n", src, i, dest, j, findWeight(g, i, j));
        }
    }
}

Node UrlListNext(Node n){
    if (n == NULL)
        return NULL;

    return n->next;
}

Node UrlListHead(UrlList ul){
    if (ul == NULL)
        return NULL;

    return ul->head;
}

char * Url(Node n){
    if (n == NULL)
        return NULL;
    
    return n->url;
}


// int main(void){
//     printf("Insert number of urls: ");
//     int size;
//     scanf("%d", &size);
//     printf("Insert collection file name: ");
//     char collection[MAX_LENGTH];
//     scanf("%s", collection);

//     LookupTable t  = newTable(size);
//     UrlList urllist = readList(collection);
//     Graph g = GetUrlGraph(urllist, t);
//     printTable(t);
//     showUrlGraph(g, t);
//     destroyUrlList(urllist);
//     destroyTable(t);
//     destroyGraph(g);
//     return 0;
// }