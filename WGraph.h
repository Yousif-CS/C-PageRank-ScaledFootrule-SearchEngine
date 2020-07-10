//Weighted Graph ADT interface ... COMP2521 

#include <stdbool.h>

#ifndef GRAPH_GUARD
#define GRAPH_GUARD
typedef struct GraphRep *Graph;

typedef struct edgeList * EdgeList;

// vertices are ints
typedef int Vertex;

// edges are pairs of vertices (end-points)
struct edge {
   Vertex v;
   Vertex w;
   double    weight;
};
typedef struct edge Edge;
#endif
Graph newGraph(int);
void  insertEdge(Graph, Edge);
void  removeEdge(Graph, Edge);
int   edgeExists(Graph, Vertex, Vertex);  // returns weight, or 0 if not adjacent
void  showGraph(Graph);
void  freeGraph(Graph);
int   graphSize(Graph);
int   reachable(Graph, Vertex);
void  destroyGraph(Graph);
int   DfsSTree(Graph, Vertex);
int   BfsSTree(Graph, Vertex);
double findWeight(Graph , Vertex, Vertex);
int   degree(Graph, Vertex);
int   vertexExists(Graph, Vertex);

EdgeList WeightedInlinks(Graph);
EdgeList WeightedOutlinks(Graph);

bool  pathExists(Graph, Vertex, Vertex);

void  components(Graph);
bool  sameComponent(Graph g, Vertex v, Vertex w);
bool  hasHamiltonianPath(Graph, Vertex, Vertex);


void  KruskalMST(Graph);

void  dijkstraPath(Graph, Vertex);
