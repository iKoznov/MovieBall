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

redBlackType(MovieNode)

//#define T MovieNode                  /* type of item to be stored */
typedef struct _MovieNode *MovieNode;
struct _MovieNode {
    Movie movie;
    SetMovieNode links;
};
char *MovieNodeId(MovieNode);
#define compLT(a,b) ( strcmp(MovieNodeId(a),MovieNodeId(b)) < 0 )
#define compEQ(a,b) ( strcmp(MovieNodeId(a),MovieNodeId(b)) == 0 )

redBlack(MovieNode)


redBlackType(int)
//#define T int                  /* type of item to be stored */
#define compLT(a,b) ( a < b )
#define compEQ(a,b) ( a == b )
redBlack(int)

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
    node->links = SetMakeMovieNode();
    return node;
}

char *MovieNodeId(MovieNode node) {
    return node->movie->id;
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
    NodeByMovie(&NODES,mov);
    
    MovieList list = movie_similars(mov);
    struct _MovieListElem *elem = list->first;
    
    while (elem) {
        Movie same = elem->movie;
        NodeByMovie(&NODES,same);
        
//        NodeByMovie( &(node->links), mov );
        
        printf("[%s] - %s\n", same->id, same->title);
        
        elem = elem->next;
    }
    
//    print(NODES);
    
    NodeMovieNode *iter = firstNodeMovieNode( &NODES );
    while(iter) {
        MovieNode nd = nodeDataMovieNode( iter );
        printf("%s\n", nd->movie->id);
        
        NodeMovieNode *lnk = firstNodeMovieNode(&(nd->links));
        while (lnk) {
            printf("\t%s\n", nodeDataMovieNode(lnk)->movie->id);
            lnk = nextNodeMovieNode(lnk);
        }
        
        
        iter = nextNodeMovieNode(iter);
//    nd->movie->id
    }
}



