//
//  tomatoes.c
//  tomatoes
//
//  Created by user on 4/5/13.
//  Copyright (c) 2013 iKoznov. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "tomatoes.h"
#include "mStr.h"
#include "movie.h"

#include "RedBlack.h"

typedef struct _MovieNode *MovieNode;
redBlackType(MovieNode)

typedef MovieNode MovieNodePointer;
redBlackType(MovieNodePointer)

typedef struct _Pair Pair;
redBlackType(Pair)

struct _MovieNode {
    Movie movie;
    SetMovieNodePointer links;
    unsigned char isLinked;
};

struct _Pair {
    MovieNode n1, n2;
    float strenth;
};

char *MovieNodeId(MovieNode);
#define compLT(a,b) ( strcmp(a->movie->id,b->movie->id) < 0 )
#define compEQ(a,b) ( strcmp(a->movie->id,b->movie->id) == 0 )
redBlack(MovieNode)

#define compLT(a,b) ( a < b )
#define compEQ(a,b) ( a == b )
redBlack(MovieNodePointer)

#define MIN(a,b) ((a)<(b)?(a):(b))
#define compLT(a,b) ( MIN(a.n1,a.n2) < MIN(b.n1,b.n2) )
#define compEQ(a,b) ( a.n1 == b.n1 && a.n2 == b.n2 || a.n1 == b.n2 && a.n2 == b.n1 )
redBlack(Pair)


static SetMovieNode NODES;
static SetPair PAIRS;


void MovieNodeDelete( SetMovieNode *S, NodeMovieNode *node ) {
    MovieNode movNd = nodeDataMovieNode(node);
    movie_free(movNd->movie);
    free(movNd);
    deleteNodeMovieNode(S, node);
}

MovieNode NodeByMovie( SetMovieNode *S, Movie movie )
{
    MovieNode node = malloc( sizeof(struct _MovieNode) );
    node->movie = movie;
    NodeMovieNode *res = findNodeMovieNode( S, node );
    if (res) {
        free(node);
        movie_free(movie);
//        MovieNodeDelete(res);
        return nodeDataMovieNode(res);
//        return NULL;
    };
    insertNodeMovieNode( S, node );
    node->isLinked = 0;
    node->links = SetMakeMovieNodePointer();
    return node;
}

MovieNodePointer NodeByPointer( SetMovieNodePointer *S, MovieNodePointer pointer )
{
    NodeMovieNodePointer *res = findNodeMovieNodePointer(S, pointer);
    if (res) return nodeDataMovieNodePointer(res);
    res = insertNodeMovieNodePointer( S, pointer);
    return nodeDataMovieNodePointer(res);
}

NodePair PairByNodes( MovieNode n1, MovieNode n2 )
{
    Pair pair;
    pair.n1 = n1;
    pair.n2 = n2;
    
    NodePair *res = findNodePair( &PAIRS, pair );
    if (res) {
        return *res;
    }
    pair.strenth = 0;
    res = insertNodePair( &PAIRS, pair );
    return *res;
}

struct RouteStack {
    struct RouteStack *previous;
    MovieNode node;
    int lenght;
};

struct PathCounter {
    int count;
    struct PathCounter *next;
};

void PathCount(struct PathCounter *counter, int len)
{
    int L = 1;
    while (L < len) {
        if ( !counter->next ) {
            counter->next = malloc(sizeof(struct PathCounter));
            counter->next->count = 0;
            counter->next->next = NULL;
        }
        counter = counter->next;
        L++;
    }
    counter->count++;
}

void PrintCounter(struct PathCounter *counter)
{
    int len = 0;
    while (counter) {
        printf("COUNTER[%d] : %d\n", len, counter->count);
        counter = counter->next;
        len++;
    }
}

void PathFree(struct PathCounter *counter)
{
    while (counter) {
        struct PathCounter *next = counter->next;
        free(counter);
        counter = next;
    }
}

int RouteStackContents(struct RouteStack *path, MovieNode node)
{
    while (path) {
        if ( node == path->node ) return 1;
        path = path->previous;
    }
    return 0;
}

void RouteStrackExpand(struct RouteStack *route, struct PathCounter *counter, MovieNode dist)
{
    MovieNode node = route->node;
    NodeMovieNodePointer *pN = firstNodeMovieNodePointer( &(node->links) );
    
    while (pN)
    {
        MovieNodePointer p = nodeDataMovieNodePointer(pN);
        
        if ( strcmp(p->movie->id,dist->movie->id) == 0 ) {
            PathCount(counter, route->lenght + 1);
        }
        else if ( !RouteStackContents(route, p) ) {
            struct RouteStack *newRoute = malloc( sizeof(struct RouteStack) );
            newRoute->previous = route;
            newRoute->node = p;
            newRoute->lenght = route->lenght + 1;
            RouteStrackExpand(newRoute, counter, dist);
            free( newRoute );
        }
        
        pN = nextNodeMovieNodePointer( pN );
    }
}

float Force(NodePair pair)
{
    struct RouteStack *route = malloc( sizeof(struct RouteStack) );
    route->previous = NULL;
    route->node = nodeDataPair(&pair).n1;
    route->lenght = 0;
    
    printf("BEGIN : %s\n", route->node->movie->title);
    printf("END : %s\n", nodeDataPair(&pair).n2->movie->title);
    
    struct PathCounter *counter = malloc(sizeof(struct PathCounter));
    counter->count = 0;
    counter->next = NULL;
    
    RouteStrackExpand( route, counter, nodeDataPair(&pair).n2 );
    
    float F = 0;
    int L = 1;
    struct PathCounter *C = counter;
    while (C) {
        F += powf(3.0f, -L) * ( 2.0f - powf(0.5f, C->count - 1) );
        C = C->next;
        L++;
    }
    
    PrintCounter(counter);
    PathFree(counter);
#warning !!! remember to free RouteStack !!!
    free( route );
    
    return F;
}

void buildTree(MovieNode node, int depth)
{
    if (node->isLinked) printf("already linked : %s\n", node->movie->title);
    if (depth <= 0 || node->isLinked) return;
    
    MovieList list = movie_similars(node->movie);
    struct _MovieListElem *elem = list->first;
    
    while (elem) {
        Movie same = elem->movie;
        MovieNode sameNode = NodeByMovie(&NODES,same);
        
        NodeByPointer( &(node->links), sameNode );
        NodeByPointer( &(sameNode->links), node );
        
//        printf("-> [%s] - %s\n", same->id, same->title);
        buildTree(sameNode, depth-1);
        
        elem = elem->next;
    }
    
    node->isLinked = 1;
    
//    elem = list->first;
//    while (elem) {
//        NodeByMovie(&NODES,elem->movie);
//    }
}

void tomatoes()
{
    puts(__PRETTY_FUNCTION__);
    
    hiMovieNode();
    
//    insertNode( MovieNO );
    printf("RAND_MAX : %d\n", RAND_MAX);
    srand( (unsigned)time(NULL) );
    
    NODES = SetMakeMovieNode();
    PAIRS = SetMakePair();
    
    Movie mov = movie_make( NULL, strdup("American+Gangster") );
    movie_reqId(mov);
    MovieNode node = NodeByMovie(&NODES,mov);
    
    buildTree(node, 2);
    
//    print(NODES);
    
    NodeMovieNode *iter = firstNodeMovieNode( &NODES );
    while(iter) {
        MovieNode nd = nodeDataMovieNode( iter );
        printf("[%s] - %s\n", nd->movie->id, nd->movie->title);
        
        NodeMovieNodePointer *lnk = firstNodeMovieNodePointer(&(nd->links));
        while (lnk) {
            printf("\t[%s] - %s\n", nodeDataMovieNodePointer(lnk)->movie->id, nodeDataMovieNodePointer(lnk)->movie->title);
            lnk = nextNodeMovieNodePointer(lnk);
        }
        
        
        
        iter = nextNodeMovieNode(iter);
//    nd->movie->id
    }
    
    NodeMovieNodePointer *lnk = firstNodeMovieNodePointer ( &(node->links) );
    NodePair pair = PairByNodes( node, nodeDataMovieNodePointer(lnk) );
    printf( "FORCE : %f\n", Force(pair) );
}



