//ADT for a lookup table of strings (urls)

typedef struct lookupTable * LookupTable;
typedef char * Item;

#define MAX_LENGTH 100
#define NOITEM NULL
#define GREATER 1
#define SMALLER 0
#define NOT_FOUND -1

LookupTable newTable(int);

int tableSize(LookupTable);

int tableInsert(LookupTable, char *);

int tableFetch(LookupTable, char *);

Item tableFetchKey(LookupTable t, int i);

void tableDelete(LookupTable, char *);

void printTable(LookupTable);
void destroyTable(LookupTable);