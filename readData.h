//ADT for reading a collection of files into a linked list

#include "WGraph.h"
#include "lookupTable.h"

typedef struct urlList * UrlList;
typedef struct node * Node;

UrlList newUrlList(void);

UrlList readList(char *);

Node UrlListNext(Node);

Node UrlListHead(UrlList);

char * Url(Node);

int UrlListSize(UrlList);

void destroyUrlList(UrlList);

Graph getUrlGraph(UrlList, LookupTable);

//for debugging purposes

void showUrlGraph(Graph, LookupTable);
