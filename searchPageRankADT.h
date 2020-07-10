//ADT for ranking pages using page rank and search word count

typedef struct urlNode UrlNode;

typedef struct urlList * UrlList;

struct urlNode{
    char * url;
    double PR;
    int SwCount;
};


UrlList newurlList(int);

//Adds a url node containing the url string and the pagerank
void urlListAdd(UrlList, char *, double);

//increments the number of search words occuring in the url
void incrementSwCount(UrlList, char *, int);

//sorts a segment by the number of search words in each url
void sortBySwCount(UrlList, int, int);

//sorts a segment by pagerank
void sortByPR(UrlList, int, int);

//fetch a node at a certain index
UrlNode urlListNode(UrlList, int);

int urlListNumElements(UrlList);
//destroy list
void destroyurlList(UrlList);

//debugging purposes
void printurlList(UrlList);