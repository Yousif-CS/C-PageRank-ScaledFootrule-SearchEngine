//ADT to contain lists



typedef struct list * List;

struct list{
    char **l;
    int size;
    int nElements;
};

List newRankList(int);

void destroyRankList(List);

//no duplicates
void RankListAdd(List, char *);

//list fetch index of char
int RankListFetch(List, char *);

//read list from file

void RankListRead(List, char *);

//union two lists

List UnionList(List *, int);

//debugging purposes
void printRankList(List);