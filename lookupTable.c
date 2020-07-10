//Implementing the lookupTable as a hash table
//the hashing algorithm used is double hashing, which I sourced from week7's lecture slides

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "lookupTable.h"

struct lookupTable{
    Item * table;
    int nSlots;
    int nElements;
    int nhash2;
};


static int isPrime(int n){
    /*
    returns 1 if number is prime, 0 otherwise
    */
    int tmp = n;
    //get rid of all factors of 2
    while (tmp % 2 == 0)
        tmp /= 2;
    //get rid of all factors of 3
    while (tmp % 3 == 0)
        tmp /= 3;
    //check the possible factors of increments of 2 from 3 if any
    for (int i = 3; i * i <= tmp; i += 2)
        if (tmp % i == 0)
            tmp /= i;
    //number unchanged; is prime
    if (tmp == n)
        return 1;
    return 0;
}

static int makeSmallerPrime(int n){
    if (n <= 2)
        return 2;
    if (isPrime(n))
        return n;
    return makeSmallerPrime(n - 1);

}
static int makeGreaterPrime(int n){
    return makeSmallerPrime(2 * n);
}
static int suitablePrime(int n, int config){
    if ((n == 2) || (n == 3))
        return n;

    if (config == GREATER)
        return makeGreaterPrime(n);
    else if (n - makeSmallerPrime(n) < 0.25 * n)
        return makeSmallerPrime(n/2);

    return -1;
}

static int hash(char * key, int size){
    /*
    sophisticated remainder hash
    */
    int sum = 0;
    int prime = 127;
    for (char *c = key; *c != '\0'; c++){
        sum = (prime * sum + *c) % size;
    }
    return sum % size;
}
static int hash2(char *key, int size){
    /*
    Seconday double hash function
    */
    int sum = 0;
    for(char * c = key; *c != '\0'; c++){
        sum += *c;
    }
    return sum % size + 1;
}

LookupTable newTable(int size){
    /*
    make a new hash table
    */
    assert(size != 0);
    LookupTable t = malloc(sizeof(*t));
    assert(t != NULL);
    //make a prime size greater than the given size
    t->table = malloc(size * sizeof(Item));
    for(int i = 0; i < size; i++)
        t->table[i] = NOITEM;
    
    t->nSlots = size;
    t->nElements = 0;
    t->nhash2 = suitablePrime(size, SMALLER);
    
    return t;
}

int tableInsert(LookupTable t, char *key){
    /*
    inserts a key into the table
    */
    assert(t->nElements <= t->nSlots);

    int i, j, h = hash(key, t->nSlots);
    //second hashing which hashes based on the prime nhash2
    int incr = hash2(key, t->nhash2);
    for(j = 0; j < t->nSlots; j += incr){
        i = (h + j) % t->nSlots;
        if (t->table[i] == NOITEM) break;
        if (strcmp(t->table[i], key) == 0) break;
    }
    if(t->table[i] == NOITEM)
        t->nElements++;
    else
        free(t->table[i]);
    t->table[i] = malloc((strlen(key) + 1)* sizeof(char));
    t->table[i] = strcpy(t->table[i], key);
    return i;
}

int tableFetch(LookupTable t, char *key){
    /*
    searches for a certain table entry with the key
    which in our case fetches the index
    */
    if (t == NULL)
        return -1;

    int i, j, h = hash(key, t->nSlots);
    int incr = hash2(key, t->nhash2);
    for(j = 0; j < t->nSlots; j+= incr){
        i = (h + j) % t->nSlots;
        if (strcmp(t->table[i], key) == 0)
            return i;
    }
    return NOT_FOUND;

}
Item tableFetchKey(LookupTable t, int i){
    /*
    fetches the key stored given its index
    */
    if (t == NULL)
        return NULL;

    assert (i < t->nSlots);
    return t->table[i];
}

void printTable(LookupTable t){
    /*
    print the table with the indices and keys
    */
    if (t == NULL)
        return;
    
    for (int i = 0; i < t->nSlots; i++){
        if (t->table[i] == NOITEM)
            printf("%3d -->   NOITEM\n", i);
        else
            printf("%3d -->   %s\n", i, t->table[i]);
    }
}

void tableDelete(LookupTable t, char * key){
    /*
    Deletes a key from the table making sure
    to shuffle the next adjacent keys
    */
    if (t == NULL)
        return;

    int i, j, h = hash(key, t->nSlots);
    int incr = hash2(key, t->nhash2);
    for (j = 0; j < t->nSlots; j += incr){
        i = (h + j) % t->nSlots;
        if (t->table[i] == NOITEM)
            return;
        if (strcmp(t->table[i], key) == 0)
            break;
    }
    free(t->table[i]);
    t->table[i] = NOITEM;
    t->nElements--;
    j = i + 1;
    while(t->table[j] != NOITEM){
        char tmpKey[MAX_LENGTH];
        //copy the key to insert later
        strncpy(tmpKey, t->table[j], strlen(t->table[j]) +1);
        free(t->table[j]);
        t->nElements--;
        t->table[j] = NOITEM;
        tableInsert(t, tmpKey);
        j = (j + 1) % t->nSlots;
    }

}
void destroyTable(LookupTable t){
    /*
    destroys the table by first freeing 
    the associated items
    */
    if (t == NULL)
        return;
    for (int i = 0; i < t->nSlots; i++){
        if (t->table[i] == NOITEM)
            continue;
        free(t->table[i]);
    }
    free(t->table);
    free(t);
}

int tableSize(LookupTable t){
    /*
    returns the number of slots alloted for the table
    */
    assert (t != NULL);
    return t->nSlots;
}
