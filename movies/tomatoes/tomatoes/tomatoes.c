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

#include "tomatoes.h"
#include "mStr.h"
#include "movie.h"

#include "RedBlack.h"

typedef struct _MovieNode *MovieNode;
redBlackType(MovieNode)

typedef MovieNode MovieNodePointer;
redBlackType(MovieNodePointer)

struct _MovieNode {
    Movie movie;
    SetMovieNodePointer links;
};

char *MovieNodeId(MovieNode);
#define compLT(a,b) ( strcmp(a->movie->id,b->movie->id) < 0 )
#define compEQ(a,b) ( strcmp(a->movie->id,b->movie->id) == 0 )
redBlack(MovieNode)

#define compLT(a,b) ( a < b )
#define compEQ(a,b) ( a == b )
redBlack(MovieNodePointer)

static SetMovieNode NODES;



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

void tomatoes()
{
    puts(__PRETTY_FUNCTION__);
    
    hiMovieNode();
    
//    insertNode( MovieNO );
    printf("RAND_MAX : %d\n", RAND_MAX);
    srand( (unsigned)time(NULL) );
    
    NODES = SetMakeMovieNode();
    
    Movie mov = movie_make( strdup("770672122"), strdup("abc") );
    MovieNode node = NodeByMovie(&NODES,mov);
    
    MovieList list = movie_similars(mov);
    struct _MovieListElem *elem = list->first;
    
    while (elem) {
        Movie same = elem->movie;
        MovieNode sameNode = NodeByMovie(&NODES,same);
        
        NodeByPointer( &(node->links), sameNode );
        NodeByPointer( &(sameNode->links), node );
        
        printf("[%s] - %s\n", same->id, same->title);
        
        elem = elem->next;
    }
    
//    print(NODES);
    
    NodeMovieNode *iter = firstNodeMovieNode( &NODES );
    while(iter) {
        MovieNode nd = nodeDataMovieNode( iter );
        printf("%s\n", nd->movie->id);
        
        NodeMovieNodePointer *lnk = firstNodeMovieNodePointer(&(nd->links));
        while (lnk) {
            printf("\t%s\n", nodeDataMovieNodePointer(lnk)->movie->id);
            lnk = nextNodeMovieNodePointer(lnk);
        }
        
        
        iter = nextNodeMovieNode(iter);
//    nd->movie->id
    }
}



