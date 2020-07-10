//Calculate the pagerank of each url given
//the pagerank object is an array

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "readData.h"
#include "EdgeList.h"
#include "PageRankADT.h"


PageRank PageRankW(double, double, int, Graph, UrlList, LookupTable);

int main(int argc, char *argv[]){

    if (argc < 4){
        fprintf(stderr, "Usage:%s [d] [diffPR] [maxIterations]\n", argv[0]);
        return 1;
    }
    //required variables for pagerank
    double dampingFactor = atof(argv[1]);
    double diffPR = atof(argv[2]);
    int maxIterations = atoi(argv[3]);

    //output file
    char outputFile[MAX_LENGTH] = "pagerankList.txt";
    //collection file
    char collectionFile[MAX_LENGTH] = "collection.txt";

    //required for creating the graph
    UrlList urllist = readList(collectionFile);
    LookupTable t = newTable(UrlListSize(urllist));
    Graph urlgraph = getUrlGraph(urllist, t);

    //calculating the pagerank
    PageRank pr = PageRankW(dampingFactor, diffPR, maxIterations,
              urlgraph, urllist, t);
    
    //sorting
    PageRankSort(pr);


    //outputing the pagerank
    FILE *fp = fopen(outputFile, "w");
    if (fp == NULL){
        fprintf(stderr, "Cannot Open output file\n");
        return 1;
    }
    outputPageRank(fp, pr, urlgraph, t);
    
    //cleanup
    fclose(fp);
    destroyPageRank(pr);
    destroyGraph(urlgraph);
    destroyUrlList(urllist);
    destroyTable(t);
    return 0;
}


PageRank PageRankW(double d, double diffPR, int maxIterations, Graph g, UrlList ul, LookupTable t){
    /*
    Creates a PageRank object that ranks the vertices of g 
    */
    Node tmp = UrlListHead(ul);
    
    int numUrls = UrlListSize(ul);
    PageRank pr = newPageRank(numUrls);

    //edge lists containing edges with weighted inlinks 
    EdgeList Wins = WeightedInlinks(g);
    EdgeList Wouts = WeightedOutlinks(g);

    for (int i = 0; tmp != NULL; i++){
        Vertex v = tableFetch(t, Url(tmp));
        if(v == NOT_FOUND)
            continue;

        double initialPr = 1.0/numUrls;

        PageRankInsert(pr, initialPr, v);
        tmp = UrlListNext(tmp);
    }

    int it = 0; // current iteration
    double diff = diffPR; // current difference in PR
    
    // keep updating the pageranks for the urls
    while (it < maxIterations && diff >= diffPR){
        diff = 0;
        for (Vertex i = 0; i < graphSize(g); i++){
            double incremDiff = 0;
            if (!vertexExists(g, i))
                continue;
            
            double currPr = PageRankValue(pr, i);
            double weightedSum = 0;
            for (Vertex j = 0; j < graphSize(g); j++){
                if (!edgeExists(g, j, i))
                    continue;
                double outlinkPr = PageRankValue(pr, j);
                double weightIn = EdgeListWeight(Wins, j, i);
                double weightOut = EdgeListWeight(Wouts, j, i);
                weightedSum += outlinkPr * weightIn * weightOut;
            }
            double nextPr = (1 - d)/numUrls + d * weightedSum;
            PageRankUpdate(pr, nextPr, i);
            incremDiff = fabs(nextPr - currPr);
            diff += incremDiff;
        }
        it++;
    }
    destroyEdgeList(Wins);
    destroyEdgeList(Wouts);
    return pr;
}

