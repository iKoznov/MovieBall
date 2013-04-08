//
//  tomatoes.c
//  tomatoes
//
//  Created by user on 4/5/13.
//  Copyright (c) 2013 iKoznov. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "tomatoes.h"
#include "mStr.h"
#include "movie.h"

#include <algorithm>
#include <list>

using namespace std;

typedef struct _MovieTreeNode *MovieTreeNode;
struct _MovieTreeNode {
    list<MovieTreeNode>nodes;
    Movie movie;
};

//struct _MovieTreeNodeListElem {
//    struct _MovieTreeNode *node;
//    struct _MovieTreeNodeListElem *next;
//};
//
//struct _MovieTreeNodeList {
//    struct _MovieTreeNodeList *first;
//    struct _MovieTreeNodeList *last;
//    size_t count;
//};

//struct _MovieListElem {
//    Movie movie;
//    struct _MovieListElem *next;
//};
//
//typedef struct _MovieList *MovieList;
//struct _MovieList {
//    struct _MovieListElem *first;
//    struct _MovieListElem *last;
//    size_t count;
//};

//#define List( Type ) \
//struct _##Type##ListElem { \
//    Type elem; \
//    struct _MovieListElem *next; \
//}; \
//typedef struct _##Type##List *Type##List; \
//struct _##Type##List { \
//    struct _##Type##ListElem *first; \
//    struct _##Type##ListElem *last; \
//    size_t count; \
//};
//
//List( MovieTreeNode )

void tomatoes()
{
    puts(__PRETTY_FUNCTION__);
    
    Movie m = movie_make( "771188157", strdup("american+Beauty") );
//    movie_reqId( m );
    
//    printf( "        id : %s\n", m->id );
//    printf( "     title : %s\n", m->title );
    
    MovieList similars = movie_similars( m );
    
    struct _MovieListElem *elem = similars->first;
    while ( elem ) {
        Movie movie = elem->movie;
        printf( "        id : %s\n", movie->id );
        printf( "     title : %s\n", movie->title );
        elem = elem->next;
    }
    
    MovieListFree( similars );
    movie_free( m );
}



