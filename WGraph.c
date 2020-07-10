//This ADT is source from the lectures of week 6 
//however has been adjusted by me and a lot of 
//ADTs and data structures have been added

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include "WGraph.h"
#include "EdgeList.h"
#include "List.h"

#define MAX_VERT 100

double dist[MAX_VERT];
int pred[MAX_VERT];
int visited[MAX_VERT];

#define SUCCESSFUL true

struct GraphRep {
    List * edges;
    int nE;
    int nV;
};

bool equalEdges(Edge e1, Edge e2){
    return e1.v == e2.v &&
           e1.w == e2.w;
}


static bool isValidEdge(Graph g, Edge e){
    return g != NULL && e.w < g->nV && e.v < g->nV;
}

Graph newGraph(int V){
    assert(V >= 0);
    
    Graph g = malloc(sizeof(struct GraphRep));
    assert(g != NULL);

    g->nE = 0;
    g->nV = V;

    g->edges = malloc(V * sizeof(List));
    for (int i = 0; i < V; i++)
        g->edges[i] = newList();
    return g;
}

void insertEdge(Graph g, Edge e){
    /*
    Adds a directed edge from `src` to `dest`
    If edge exists, it modifies the weight
    */
    assert(isValidEdge(g, e));
    if (e.v == e.w)
        return;
    int isAdded = insertLL(g->edges[e.v], e.w, e.weight);
    if (isAdded)
        g->nE++;
}

double findWeight(Graph g, Vertex v, Vertex w){
    /*
    Find the weight of the edge between v, w, if it exists
    returns -1 if edge does not exist
    */
    if (!edgeExists(g, v, w))
        return -1;
    Node n = fetchNode(g->edges[v], w);
    return weight(n);

}
int edgeExists(Graph g, Vertex src, Vertex dest){
    /*
    Checks if a directed edge exists from `src` to `dest`
    */
    return inLL(g->edges[src], dest);
}

int degree(Graph g, Vertex v){
    if (g == NULL)
        return -1;
    
    return size(g->edges[v]);
}

int vertexExists(Graph g, Vertex v){
    if (g == NULL)
        return 0;
    return v < g->nV;
}
static int doReachable(Graph g, Vertex v, int visited[], int compID){
    visited[v] = compID;
    int count = 0;
    for (int i = 0; i < g->nV; i++){
        if (edgeExists(g, v, i) && visited[i] != -1){
            count += doReachable(g, i, visited, compID);
        }
    }
    return count + 1;
}
int reachable(Graph g, Vertex v){
    memset(visited, -1, g->nV);
    return doReachable(g, v, visited, -1);
}

int doDfsPath(Graph g, Vertex v){
    int count = 0;
    for (int i = 0; i < g->nV; i++){
        if (edgeExists(g, v, i) && visited[i] == -1){
            visited[i] = v;
            count += doDfsPath(g, i);
        }
    }
    return count + 1;
}
int DfsSTree(Graph g, Vertex start){
    memset(visited, -1, g->nV);
    visited[start] = start;
    return doDfsPath(g, start);
}

int graphSize(Graph g){
    assert (g != NULL);
    return g->nV;
}


EdgeList GraphEdgeList(Graph g){
    /*
    Make an Edgelist from a graph
    */
    if (g == NULL)
        return NULL;
    EdgeList el = newEdgeList();
    for(int i = 0; i < g->nV; i++){
        Node curr = head(g->edges[i]);
        while(curr != NULL){
            Edge e; e.v = i; e.w = value(curr);
            e.weight = weight(curr);
            EdgeListAdd(el, e);
            curr = next(curr);
        }

    }
    return el;
}



static double outlinksReference(Graph g, Vertex ref){
    /*
    Calculates the number of outlinks of all adjacent vertices of `ref`
    */
    double count = 0;
    for(int adj = 0; adj < graphSize(g); adj++){
        if(edgeExists(g, ref, adj))
            count += (degree(g, adj) == 0)? 0.5 : degree(g, adj);
    }
    return count;
}

static int inLinksCount(Graph g, Vertex v){

    int count = 0;

    for(int i = 0; i < graphSize(g); i++){
        if (edgeExists(g, i, v))
            count++;
    }
    return count;
}

EdgeList WeightedInlinks(Graph g){
    EdgeList el = GraphEdgeList(g);

    EdgeList updated = newEdgeList();

    while(!EdgeListEmpty(el)){

        Edge e = EdgeListPop(el);

        // calculate the number of inlinks of all
        // the reference vertices of the source vertex in the edge
        double inlinksDest = 0;
        double totalInlinks = 0;
        for (Vertex i = 0; i < graphSize(g); i++){
            if (!edgeExists(g, e.v, i))
                continue;
            
            double inLinks = inLinksCount(g, i);
            inLinks = (inLinks == 0)? 0.5 : inLinks;
            
            if (e.w == i)
                inlinksDest = inLinks;

            totalInlinks = totalInlinks + inLinks;

        }
        e.weight = (inlinksDest == 0)? 0.5/totalInlinks : inlinksDest/totalInlinks;
        EdgeListAdd(updated, e);
    }
    destroyEdgeList(el);
    return updated;
}

EdgeList WeightedOutlinks(Graph g){
    
    //create a list of Edge objects from graph
    EdgeList el = GraphEdgeList(g);
    //for each edge, calculate its weight
    EdgeList updated = newEdgeList();
    while(!EdgeListEmpty(el)){

        Edge e = EdgeListPop(el);
        
        //calculate the total number of reference outlinks
        double outlinksTotal = outlinksReference(g, e.v);

        //calculate new weight
        double newWeight = (degree(g, e.w) == 0)? 0.5/outlinksTotal : (double)(degree(g, e.w)/outlinksTotal);
        e.weight = newWeight;
        
        //add the updated edge
        EdgeListAdd(updated, e);

    } 
    destroyEdgeList(el);
    return updated;
}


void destroyGraph(Graph g){
    for (int i = 0; i < graphSize(g); i++){
        destroyList(g->edges[i]);
    }
    free(g->edges);
    free(g);
}
