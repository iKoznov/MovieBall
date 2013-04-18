//
//  tomatoes.h
//  tomatoes
//
//  Created by user on 4/8/13.
//  Copyright (c) 2013 iKoznov. All rights reserved.
//

#ifndef __tomatoes__tomatoes__
#define __tomatoes__tomatoes__

//#include <iostream>
#include "movie.h"

typedef struct Node_ Node;
typedef struct Node *Set;
typedef struct _MovieNode *MovieNode;
struct _MovieNode {
    Movie movie;
    Set links;
};

char *MovieNodeId(MovieNode);
Set SetMake();
MovieNode nodeData(Node *);
Node *findNode(Set *,MovieNode data);
Node *insertNode(Set *,MovieNode data);
void deleteNode(Set *,Node *z);
Node *firstNode(Set *);
Node *nextNode(Node *);
void print(Node *);

void tomatoes();

#endif /* defined(__tomatoes__tomatoes__) */
