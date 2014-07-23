//
//  tomatoes.h
//  tomatoes
//
//  Created by user on 4/8/13.
//  Copyright (c) 2013 iKoznov. All rights reserved.
//

#ifndef __tomatoes__tomatoes__
#define __tomatoes__tomatoes__

#ifdef __cplusplus
extern "C"
{
#endif

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
    
Movie tomatoes( const char *const query );
MovieNode NodeByMovie( Movie movie );
NodePair *PairByNodes( MovieNode n1, MovieNode n2 );
float Force(NodePair *pair);
SetMovieNode *getNODES();
    
#ifdef __cplusplus
}
#endif

#endif /* defined(__tomatoes__tomatoes__) */
