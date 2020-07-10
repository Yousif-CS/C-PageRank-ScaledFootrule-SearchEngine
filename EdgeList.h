//ADT for managing array of edges representation
#include "WGraph.h"

typedef struct edgeList *EdgeList;
typedef struct edgeNode * EdgeNode;

EdgeList newEdgeList();

EdgeList sortEdgeList(EdgeList);

Edge EdgeListPop(EdgeList);

double EdgeListWeight(EdgeList, Vertex, Vertex);

EdgeNode EdgeListHead(EdgeList);

EdgeNode EdgeListNext(EdgeNode);

Edge EdgeNodeEdge(EdgeNode);

int EdgeListEmpty(EdgeList);

int EdgeListAdd(EdgeList, Edge);


void destroyEdgeList(EdgeList);

//debugging...
void printEdgeList(EdgeList);