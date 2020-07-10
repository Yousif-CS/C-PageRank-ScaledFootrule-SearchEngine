#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


#include "EdgeList.h"
typedef struct edgeNode * EdgeNode;
typedef struct node * Node;

struct edgeNode{
    Edge edge;
    EdgeNode next;
};
struct edgeList{
    EdgeNode head;
    EdgeNode tail;
    int nElements;    
};

EdgeList newEdgeList(){
    EdgeList l = malloc(sizeof(*l));
    assert(l != NULL);
    l->head = l->tail = NULL;
    l->nElements = 0;
    return l;
}

static EdgeNode newEdgeListNode(Edge e){
    EdgeNode n = malloc(sizeof(*n));
    assert(n != NULL);

    n->edge = e;
    n->next = NULL;
    return n;
}

int EdgeListAdd(EdgeList l, Edge e){
    if (l == NULL)
        return 0;
    EdgeNode n = newEdgeListNode(e);
    if (l->nElements == 0)
        l->head = l->tail = n;
    else
        l->tail->next = n;
    l->tail = n;
    l->nElements++;
    return 1;
    
}

Edge EdgeListPop(EdgeList l){
    /*
    remove from the head
    returns an edge of -1 values when empty
    */
    if (l == NULL){
        Edge e; e.v = -1; e.w = -1; e.weight = -1;
        return e;
    }
    if(EdgeListEmpty(l)){
        Edge e; e.v = -1; e.w = -1; e.weight = -1;
        return e;
    }
    EdgeNode n = l->head;
    l->head = l->head->next;
    Edge toReturn = n->edge;
    free(n);
    l->nElements--;
    return toReturn;
}

static void doDestroyEdgeList(EdgeNode n){
    if (n == NULL)
        return;
    doDestroyEdgeList(n->next);
    free(n);
}
void destroyEdgeList(EdgeList l){
    if (l == NULL)
        return;
    doDestroyEdgeList(l->head);
    free(l);
}

int EdgeListEmpty(EdgeList l){
    if (l == NULL)
        return -1;
    return l->nElements == 0;
}

EdgeNode EdgeListHead(EdgeList el){
    if (el == NULL)
        return NULL;
    return el->head;
}
EdgeNode EdgeListNext(EdgeNode en){
    if (en == NULL)
        return NULL;
    return en->next;
}
Edge EdgeNodeEdge(EdgeNode en){
    if (en == NULL){
        Edge e; e.v = -1; e.w = -1; e.weight = -1;
        return e;
    }
    return en->edge;
}

double EdgeListWeight(EdgeList el, Vertex src, Vertex dest){
    /*
    Return the weight of an edge 
    */
    EdgeNode tmp = EdgeListHead(el);
    while(tmp != NULL){
        if (tmp->edge.v == src && tmp->edge.w == dest)
            return tmp->edge.weight;
        tmp = EdgeListNext(tmp);
    }
    return -1;
}

void printEdgeList(EdgeList l){
    if (l == NULL)
        return;
    
    EdgeNode curr = l->head;
    while(curr != NULL){
        printf("%d ->   %d  | %7lf\n", curr->edge.v, curr->edge.w, curr->edge.weight);
        curr = curr->next;
    }
}
