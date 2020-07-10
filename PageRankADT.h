//ADT for PageRank and with API for Graph and LookupTable ADT

#include "WGraph.h"
#include "lookupTable.h"

typedef struct _pageRankNode PageRankNode;

typedef struct pageRank * PageRank;

//create a new PageRank list
PageRank newPageRank(int);

//inserts a struct with the node and its pagerank to the list
void PageRankInsert(PageRank, double, Vertex);

void destroyPageRank(PageRank);

void PageRankUpdate(PageRank, double, Vertex);

//fetches the pagerank of a node 
double PageRankValue(PageRank, Vertex);

int PageRankSize(PageRank);

void PageRankSort(PageRank);

void outputPageRank(FILE *, PageRank, Graph, LookupTable);

//debugging purposes
void printPageRank(PageRank);