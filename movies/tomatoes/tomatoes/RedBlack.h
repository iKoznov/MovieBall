//
//  RedBlackMovie.c
//  tomatoes
//
//  Created by user on 4/17/13.
//  Copyright (c) 2013 iKoznov. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tomatoes.h"

//#define T MovieNode                  /* type of item to be stored */
//#define compLT(a,b) ( strcmp(MovieNodeId(a),MovieNodeId(b)) < 0 )
//#define compEQ(a,b) ( strcmp(MovieNodeId(a),MovieNodeId(b)) == 0 )

/* Red-Black tree description */
typedef enum { BLACK, RED } nodeColor;

#define redBlackType(T)\
typedef struct Node##T *Set##T;

#define redBlack(T)\
void hi##T(){puts(#T);}\
\
typedef struct Node_##T Node##T;\
struct Node_##T {\
    struct Node_##T *left;         /* left child */\
    struct Node_##T *right;        /* right child */\
    struct Node_##T *parent;       /* parent */\
    nodeColor color;            /* node color (BLACK, RED) */\
    T data;                     /* data stored in node */\
};\
\
T nodeData##T( Node##T *node ) {\
    return node->data;\
}\
\
Node##T sentinel##T = { &sentinel##T, &sentinel##T, 0, BLACK, 0 };\
\
Set##T SetMake##T() {\
    return (Set##T)&sentinel##T;\
}\
\
void rotateLeft##T( Set##T *R, Node##T *x ) {\
\
    Node##T *y = x->right;\
    \
    /* establish x->right link */\
    x->right = y->left;\
    if (y->left != &sentinel##T) y->left->parent = x;\
    \
    /* establish y->parent link */\
    if (y != &sentinel##T) y->parent = x->parent;\
    if (x->parent) {\
        if (x == x->parent->left)\
            x->parent->left = y;\
        else\
            x->parent->right = y;\
    } else {\
        *R = (Set##T)y;\
    }\
    \
    /* link x and y */\
    y->left = x;\
    if (x != &sentinel##T) x->parent = y;\
}\
\
void rotateRight##T( Set##T *R, Node##T *x ) {\
\
    Node##T *y = x->left;\
    \
    /* establish x->left link */\
    x->left = y->right;\
    if (y->right != &sentinel##T) y->right->parent = x;\
    \
    /* establish y->parent link */\
    if (y != &sentinel##T) y->parent = x->parent;\
    if (x->parent) {\
        if (x == x->parent->right)\
            x->parent->right = y;\
        else\
            x->parent->left = y;\
    } else {\
        *R = (Set##T)y;\
    }\
    \
    /* link x and y */\
    y->right = x;\
    if (x != &sentinel##T) x->parent = y;\
}\
\
void insertFixup##T( Set##T *R, Node##T *x ) {\
    /* check Red-Black properties */\
    while (x != (Node##T *)*R && x->parent->color == RED) {\
        /* we have a violation */\
        if (x->parent == x->parent->parent->left) {\
            Node##T *y = x->parent->parent->right;\
            if (y->color == RED) {\
                \
                /* uncle is RED */\
                x->parent->color = BLACK;\
                y->color = BLACK;\
                x->parent->parent->color = RED;\
                x = x->parent->parent;\
            } else {\
                \
                /* uncle is BLACK */\
                if (x == x->parent->right) {\
                    /* make x a left child */\
                    x = x->parent;\
                    rotateLeft##T(R,x);\
                }\
                \
                /* recolor and rotate */\
                x->parent->color = BLACK;\
                x->parent->parent->color = RED;\
                rotateRight##T(R,x->parent->parent);\
            }\
        } else {\
            \
            /* mirror image of above code */\
            Node##T *y = x->parent->parent->left;\
            if (y->color == RED) {\
                \
                /* uncle is RED */\
                x->parent->color = BLACK;\
                y->color = BLACK;\
                x->parent->parent->color = RED;\
                x = x->parent->parent;\
            } else {\
                \
                /* uncle is BLACK */\
                if (x == x->parent->left) {\
                    x = x->parent;\
                    rotateRight##T(R,x);\
                }\
                x->parent->color = BLACK;\
                x->parent->parent->color = RED;\
                rotateLeft##T(R,x->parent->parent);\
            }\
        }\
    }\
    ((Node##T*)*R)->color = BLACK;\
}\
\
Node##T *insertNode##T( Set##T *R, T data ) {\
    Node##T *current, *parent, *x;\
    /* find where node belongs */\
    current = (Node##T *)*R;\
    parent = 0;\
    while (current != &sentinel##T) {\
        if (compEQ(data, current->data)) return (current);\
        parent = current;\
        current = compLT(data, current->data) ?\
        current->left : current->right;\
    }\
    \
    /* setup new node */\
    if ((x = malloc (sizeof(*x))) == 0) {\
        printf ("insufficient memory (insertNode)\n");\
        exit(1);\
    }\
    x->data = data;\
    x->parent = parent;\
    x->left = &sentinel##T;\
    x->right = &sentinel##T;\
    x->color = RED;\
    \
    /* insert node in tree */\
    if(parent) {\
        if(compLT(data, parent->data))\
            parent->left = x;\
        else\
            parent->right = x;\
    } else {\
        *R = (Set##T)x;\
    }\
    \
    insertFixup##T(R,x);\
    return(x);\
}\
\
void deleteFixup##T( Set##T *R, Node##T *x ) {\
    while (x != (Node##T *)*R && x->color == BLACK) {\
        if (x == x->parent->left) {\
            Node##T *w = x->parent->right;\
            if (w->color == RED) {\
                w->color = BLACK;\
                x->parent->color = RED;\
                rotateLeft##T (R,x->parent);\
                w = x->parent->right;\
            }\
            if (w->left->color == BLACK && w->right->color == BLACK) {\
                w->color = RED;\
                x = x->parent;\
            } else {\
                if (w->right->color == BLACK) {\
                    w->left->color = BLACK;\
                    w->color = RED;\
                    rotateRight##T (R,w);\
                    w = x->parent->right;\
                }\
                w->color = x->parent->color;\
                x->parent->color = BLACK;\
                w->right->color = BLACK;\
                rotateLeft##T (R,x->parent);\
                x = (Node##T *)*R;\
            }\
        } else {\
            Node##T *w = x->parent->left;\
            if (w->color == RED) {\
                w->color = BLACK;\
                x->parent->color = RED;\
                rotateRight##T (R,x->parent);\
                w = x->parent->left;\
            }\
            if (w->right->color == BLACK && w->left->color == BLACK) {\
                w->color = RED;\
                x = x->parent;\
            } else {\
                if (w->left->color == BLACK) {\
                    w->right->color = BLACK;\
                    w->color = RED;\
                    rotateLeft##T (R,w);\
                    w = x->parent->left;\
                }\
                w->color = x->parent->color;\
                x->parent->color = BLACK;\
                w->left->color = BLACK;\
                rotateRight##T (R,x->parent);\
                x = (Node##T *)*R;\
            }\
        }\
    }\
    x->color = BLACK;\
}\
\
void deleteNode##T( Set##T *R, Node##T *z ) {\
    Node##T *x, *y;\
    if (!z || z == &sentinel##T) return;\
\
    if (z->left == &sentinel##T || z->right == &sentinel##T) {\
        /* y has a NIL node as a child */\
        y = z;\
    } else {\
        /* find tree successor with a NIL node as a child */\
        y = z->right;\
        while (y->left != &sentinel##T) y = y->left;\
    }\
    \
    /* x is y's only child */\
    if (y->left != &sentinel##T)\
        x = y->left;\
    else\
        x = y->right;\
    \
    /* remove y from the parent chain */\
    x->parent = y->parent;\
    if (y->parent)\
        if (y == y->parent->left)\
            y->parent->left = x;\
        else\
            y->parent->right = x;\
        else\
            *R = (Set##T)x;\
    \
    if (y != z) z->data = y->data;\
    \
    if (y->color == BLACK)\
        deleteFixup##T (R,x);\
    \
    free (y);\
}\
\
Node##T *findNode##T( Set##T *R, T data ) {\
    Node##T *current = (Node##T *)*R;\
    while(current != &sentinel##T)\
        if(compEQ(data, current->data))\
            return (current);\
        else\
            current = compLT (data, current->data) ?\
            current->left : current->right;\
    return(0);\
}\
\
Node##T *firstNode##T(Set##T *R) {\
    Node##T *current = (Node##T *)*R;\
    if(current != &sentinel##T) {\
        while (current->left != &sentinel##T) {\
            current = current->left;\
        }\
        return current;\
    }\
    return(0);\
}\
\
Node##T *nextNode##T(Node##T *N) {\
    if (N->right != &sentinel##T) {\
        N = N->right;\
        while (N->left != &sentinel##T)\
            N = N->left;\
        return N;\
    }\
    while (N->parent) {\
        if (N == N->parent->left)\
            return N->parent;\
        N = N->parent;\
    }\
    return 0;\
}
