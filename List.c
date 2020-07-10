#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "List.h"

#define NOITEM -1

struct node{
    int v;
    double weight;
    Node next;
};

struct list {
    Node head;
    Node tail;
    int degree;
};


List newList(){
    /*
    create a new list 
    */
    List l = malloc(sizeof(*l));
    assert(l != NULL);
    l->head = NULL;
    l->tail = NULL;
    l->degree = 0;

    return l;
}
static int doInLL(Node n, int w){
    if (n == NULL)
        return 0;
    if (n->v == w)
        return 1;
    else 
        return doInLL(n->next, w);

}
int inLL(List l, int w){
    /*
    Return whether a node is in the list l
    */
    return doInLL(l->head, w);
}

static Node newListNode(int v, double weight){
    Node new = malloc(sizeof(*new));
    assert(new != NULL);
    new->v = v;
    new->weight = weight;
    new->next = NULL;
    return new;
}

static int doInsertLL(Node n, int v, double weight){
    /*
    inserts a new node if doesn't exist, otherwise 
    update the weight of the link
    */
    
    if (n == NULL)
        return 0;
    if (n->v == v){
        n->weight = weight;
        return 0;
    }else if (n->next == NULL){
        n->next = newListNode(v, weight);
        return 1;
    }
    return doInsertLL(n->next, v, weight);

}
int insertDup(List l, int v, double weight){
    /*
    Insert to the list but allow duplicates
    this method uses addition to the tail but
    does not allow mixed insertion with insertLL
    
    returns 0 if unsuccessful otherwise 1
    */
    if (l == NULL)
        return 0;
    //if the user has inserted using insertLL before
    //which does not update the tail
    if (l->tail != NULL && l->tail->next != NULL)
        return 0;
    
    Node n = newListNode(v, weight);
    if (l->degree == 0){
        l->head = l->tail = n;
    }else
        l->tail->next = n;
    l->tail = n;
    l->degree++;
    return 1;

}
int insertLL(List l, int v, double weight){
    /* 
    Wrapper for a recursive insertion function
    */
    if (l == NULL)
        return -1;

    int isAdded = 0;
    if (l->head == NULL){
        l->head = newListNode(v, weight);
        isAdded = 1;
    }else
        isAdded = doInsertLL(l->head, v, weight);
    
    if (isAdded)
        l->degree++;
    return isAdded;
    
}   

int removeLLHead(List l){
    if (l == NULL)
        return NOITEM;
    if (l->head == NULL)
        return NOITEM;

    Node tmp = l->head;
    l->head = l->head->next;
    int value = tmp->v;
    free(tmp);
    l->degree--;
    return value;
}
static void doDestroyList(Node n){
    if (n == NULL)
        return;
    doDestroyList(n->next);
    free(n);
}

void destroyList(List l){
    if (l == NULL)
        return;
    doDestroyList(l->head);
    free(l);
}

static Node doFetchNode(Node n, int v){
    if (n == NULL)
        return NULL;
    if(n->v == v)
        return n;
    else return doFetchNode(n->next, v);
}
Node fetchNode(List l, int v){
    if (l == NULL)
        return NULL;

    return doFetchNode(l->head, v);
}
int size(List l){
    return l->degree;
}

double weight(Node n){
    if (n == NULL)
        return -1;
    return n->weight;
}

//debugging purposes
void printList(List l){
    if (l == NULL){
        printf("List empty!\n");
        return;
    }
    Node tmp = l->head;
    printf("HEAD => ");
    while(tmp != NULL){
        printf("%d ", tmp->v);
        tmp = tmp->next;
    }
    printf("\n");
}


Node head(List l){
    if (l == NULL)
        return NULL;
    return l->head;
}
Node next(Node n){
    return n->next;
}

int value(Node n){
    return n->v;
}
