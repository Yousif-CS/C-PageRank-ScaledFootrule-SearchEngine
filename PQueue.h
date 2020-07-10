//ADT for Priority Queue
typedef struct{
    double dist;
    int speculatedPos;
    int url;
} UrlNode;

typedef struct pQueue * PQueue;


struct pQueue{
    UrlNode *h;
    int size;
    int nElements;
};

PQueue newPQueue(int);

void PQueuePush(PQueue, UrlNode);

UrlNode PQueuePop(PQueue);

void PQueueDestroy(PQueue);

int PQueueEmpty(PQueue);


//debugging

void printPQueue(PQueue);