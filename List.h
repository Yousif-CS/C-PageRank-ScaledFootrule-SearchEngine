//simple List ADT

typedef struct node * Node; 
typedef struct list * List;



List newList();

int insertLL(List, int, double);

int insertDup(List, int, double);

int removeLLHead(List);

void destroyList(List);

int size(List);

int inLL(List, int);

Node fetchNode(List, int);

Node next(Node);

Node head(List);

int value(Node);

double weight(Node);

//debugging purposes
void printList(List);