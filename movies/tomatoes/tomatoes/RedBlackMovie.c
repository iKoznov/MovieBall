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
#include "movie.h"
#include "tomatoes.h"

typedef MovieNode T;                  /* type of item to be stored */
int compLT(T a, T b) { return strcmp(MovieNodeId(a),MovieNodeId(b)) < 0; }
int compEQ(T a, T b) { return strcmp(MovieNodeId(a),MovieNodeId(b)) == 0; }

/* Red-Black tree description */
typedef enum { BLACK, RED } nodeColor;

struct Node_ {
    struct Node_ *left;         /* left child */
    struct Node_ *right;        /* right child */
    struct Node_ *parent;       /* parent */
    nodeColor color;            /* node color (BLACK, RED) */
    T data;                     /* data stored in node */
};

T nodeData( Node *node ) {
    return node->data;
}

#define NIL &sentinel           /* all leafs are sentinels */
Node sentinel = { NIL, NIL, 0, BLACK, 0 };

//Node *root = NIL;               /* root of Red-Black tree */

Set SetMake() {
//    struct Node_ *set = malloc( sizeof(struct Node_) );
//    *set = sentinel;
//    return (Set)set;
//    Node *root = malloc(sizeof(struct Node_));
//    memcpy(root, NIL, sizeof(struct Node_));
//    return (Set)root;
//    Node *root = NIL;
    return (Set)NIL;
}

void rotateLeft( Set *R, Node *x ) {
    
    /**************************
     *  rotate node x to left *
     **************************/
    
    Node *y = x->right;
    
    /* establish x->right link */
    x->right = y->left;
    if (y->left != NIL) y->left->parent = x;
    
    /* establish y->parent link */
    if (y != NIL) y->parent = x->parent;
    if (x->parent) {
        if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
    } else {
        *R = (Set)y;
    }
    
    /* link x and y */
    y->left = x;
    if (x != NIL) x->parent = y;
}

void rotateRight( Set *R, Node *x ) {
    
    /****************************
     *  rotate node x to right  *
     ****************************/
    
    Node *y = x->left;
    
    /* establish x->left link */
    x->left = y->right;
    if (y->right != NIL) y->right->parent = x;
    
    /* establish y->parent link */
    if (y != NIL) y->parent = x->parent;
    if (x->parent) {
        if (x == x->parent->right)
            x->parent->right = y;
        else
            x->parent->left = y;
    } else {
        *R = (Set)y;
    }
    
    /* link x and y */
    y->right = x;
    if (x != NIL) x->parent = y;
}

void insertFixup( Set *R, Node *x ) {
    
    /*************************************
     *  maintain Red-Black tree balance  *
     *  after inserting node x           *
     *************************************/
    
    /* check Red-Black properties */
    while (x != (Node *)*R && x->parent->color == RED) {
        /* we have a violation */
        if (x->parent == x->parent->parent->left) {
            Node *y = x->parent->parent->right;
            if (y->color == RED) {
                
                /* uncle is RED */
                x->parent->color = BLACK;
                y->color = BLACK;
                x->parent->parent->color = RED;
                x = x->parent->parent;
            } else {
                
                /* uncle is BLACK */
                if (x == x->parent->right) {
                    /* make x a left child */
                    x = x->parent;
                    rotateLeft(R,x);
                }
                
                /* recolor and rotate */
                x->parent->color = BLACK;
                x->parent->parent->color = RED;
                rotateRight(R,x->parent->parent);
            }
        } else {
            
            /* mirror image of above code */
            Node *y = x->parent->parent->left;
            if (y->color == RED) {
                
                /* uncle is RED */
                x->parent->color = BLACK;
                y->color = BLACK;
                x->parent->parent->color = RED;
                x = x->parent->parent;
            } else {
                
                /* uncle is BLACK */
                if (x == x->parent->left) {
                    x = x->parent;
                    rotateRight(R,x);
                }
                x->parent->color = BLACK;
                x->parent->parent->color = RED;
                rotateLeft(R,x->parent->parent);
            }
        }
    }
    ((Node*)*R)->color = BLACK;
}

Node *insertNode( Set *R, T data ) {
    Node *current, *parent, *x;
    
    /***********************************************
     *  allocate node for data and insert in tree  *
     ***********************************************/
    
    /* find where node belongs */
    current = (Node *)*R;
    parent = 0;
    while (current != NIL) {
        if (compEQ(data, current->data)) return (current);
        parent = current;
        current = compLT(data, current->data) ?
        current->left : current->right;
    }
    
    /* setup new node */
    if ((x = malloc (sizeof(*x))) == 0) {
        printf ("insufficient memory (insertNode)\n");
        exit(1);
    }
    x->data = data;
    x->parent = parent;
    x->left = NIL;
    x->right = NIL;
    x->color = RED;
    
    /* insert node in tree */
    if(parent) {
        if(compLT(data, parent->data))
            parent->left = x;
        else
            parent->right = x;
    } else {
        *R = (Set)x;
    }
    
    insertFixup(R,x);
    return(x);
}

void deleteFixup( Set *R, Node *x ) {
    
    /*************************************
     *  maintain Red-Black tree balance  *
     *  after deleting node x            *
     *************************************/
    
    while (x != (Node *)*R && x->color == BLACK) {
        if (x == x->parent->left) {
            Node *w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rotateLeft (R,x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    rotateRight (R,w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                rotateLeft (R,x->parent);
                x = (Node *)*R;
            }
        } else {
            Node *w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rotateRight (R,x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    rotateLeft (R,w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rotateRight (R,x->parent);
                x = (Node *)*R;
            }
        }
    }
    x->color = BLACK;
}

void deleteNode( Set *R, Node *z ) {
    Node *x, *y;
    
    /*****************************
     *  delete node z from tree  *
     *****************************/
    
    if (!z || z == NIL) return;
    
    
    if (z->left == NIL || z->right == NIL) {
        /* y has a NIL node as a child */
        y = z;
    } else {
        /* find tree successor with a NIL node as a child */
        y = z->right;
        while (y->left != NIL) y = y->left;
    }
    
    /* x is y's only child */
    if (y->left != NIL)
        x = y->left;
    else
        x = y->right;
    
    /* remove y from the parent chain */
    x->parent = y->parent;
    if (y->parent)
        if (y == y->parent->left)
            y->parent->left = x;
        else
            y->parent->right = x;
        else
            *R = (Set)x;
    
    if (y != z) z->data = y->data;
    
    
    if (y->color == BLACK)
        deleteFixup (R,x);
    
    free (y);
}

Node *findNode( Set *R, T data ) {
    
    /*******************************
     *  find node containing data  *
     *******************************/
    
    Node *current = (Node *)*R;
    while(current != NIL)
        if(compEQ(data, current->data))
            return (current);
        else
            current = compLT (data, current->data) ?
            current->left : current->right;
    return(0);
}

void print(Node *N) {
    if (N==NIL) return;
    print(N->left);
    printf("print : %s\n", N->data->movie->id );
    print(N->right);
}

Node *firstNode(Set *R) {
    Node *current = (Node *)*R;
    if(current != NIL) {
        while (current->left != NIL) {
            current = current->left;
        }
        return current;
    }
    return(0);
}

Node *nextNode(Node *N) {
    if (N->right != NIL) {
        N = N->right;
        while (N->left != NIL)
            N = N->left;
        return N;
    }
    while (N->parent) {
        if (N == N->parent->left)
            return N->parent;
        N = N->parent;
    }
    return 0;
}
