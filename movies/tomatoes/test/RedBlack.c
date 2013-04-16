#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define RED 0
#define BLACK 1

unsigned char less(int a, int b) {
    return a < b;
}

struct node {
    int value;
    struct node *parent;
    struct node *left;
    struct node *right;
    unsigned char color;
};

struct node *treeRoot = NULL;

void insert_case1(struct node *n);
void insert_case2(struct node *n);
void insert_case3(struct node *n);
void insert_case4(struct node *n);
void insert_case5(struct node *n);

void insert(struct node *R, struct node *N) {
    if ( less(N->value, R->value) ) {
        if ( R->left ) insert( R->left, N);
        else {
            N->parent = R;
            R->left = N;
        }
    }
    else {
        if ( R->right ) insert( R->right, N );
        else {
            N->parent = R;
            R->right = N;
        }
    }
}

void insertValue(int value) {
    struct node *N = malloc( sizeof(struct node) );
    N->value = value;
    N->left = NULL;
    N->right = NULL;
    N->color = RED;
    
    if (!treeRoot) {
        N->parent = NULL;
        treeRoot = N;
    }
    else
        insert(treeRoot, N);
    
    insert_case1(N);
}

void delete(int value) {
    
}

struct node *grandparent(struct node *n) {
    if ((n != NULL) && (n->parent != NULL))
        return n->parent->parent;
    else
        return NULL;
}

struct node *uncle(struct node *n) {
    struct node *g = grandparent(n);
    if (g == NULL)
        return NULL; // No grandparent means no uncle
    if (n->parent == g->left)
        return g->right;
    else
        return g->left;
}

void rotate_left(struct node *x)
{
    /**************************
     *  rotate node x to left *
     **************************/
    
    struct node *y = x->right;
    
    /* establish x->right link */
    x->right = y->left;
    if (y->left != NULL) y->left->parent = x;
    
    /* establish y->parent link */
    if (y != NULL) y->parent = x->parent;
    if (x->parent) {
        if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
    } else {
        treeRoot = y;
    }
    
    /* link x and y */
    y->left = x;
    if (x != NULL) x->parent = y;
}

void rotate_right(struct node *x)
{
    /****************************
     *  rotate node x to right  *
     ****************************/
    
    struct node *y = x->left;
    
    /* establish x->left link */
    x->left = y->right;
    if (y->right != NULL) y->right->parent = x;
    
    /* establish y->parent link */
    if (y != NULL) y->parent = x->parent;
    if (x->parent) {
        if (x == x->parent->right)
            x->parent->right = y;
        else
            x->parent->left = y;
    } else {
        treeRoot = y;
    }
    
    /* link x and y */
    y->right = x;
    if (x != NULL) x->parent = y;
}

void insert_case1(struct node *n) {
    if (n->parent == NULL)
        n->color = BLACK;
    else
        insert_case2(n);
}

void insert_case2(struct node *n) {
    if (n->parent->color == BLACK)
        return; /* Tree is still valid */
    else
        insert_case3(n);
}

void insert_case3(struct node *n) {
    struct node *u = uncle(n), *g;
    
    if ((u != NULL) && (u->color == RED) && (n->parent->color == RED)) {
        n->parent->color = BLACK;
        u->color = BLACK;
        g = grandparent(n);
        g->color = RED;
        insert_case1(g);
    } else {
        insert_case4(n);
    }
}

void insert_case4(struct node *n) {
    struct node *g = grandparent(n);
    
    if ((n == n->parent->right) && (n->parent == g->left)) {
        rotate_left(n->parent);
        n = n->left;
    } else if ((n == n->parent->left) && (n->parent == g->right)) {
        rotate_right(n->parent);
        n = n->right;
    }
    insert_case5(n);
}

void insert_case5(struct node *n) {
    struct node *g = grandparent(n);
    
    n->parent->color = BLACK;
    g->color = RED;
    if ((n == n->parent->left) && (n->parent == g->left)) {
        rotate_right(g);
    } else { /* (n == n->parent->right) and (n->parent == g->right) */
        rotate_left(g);
    }
}

void print(struct node *N) {
    if (!N) return;
    print(N->left);
    printf("%d\n", N->value);
    print(N->right);
}

int main()
{
	puts("Red Black Tree");
    
    printf("RAND_MAX : %d\n", RAND_MAX);
    srand( time(NULL) );
    
    clock_t t0 = clock();
    
    for (unsigned i = 0; i < 100000; i++) {
        int val = rand();
        insertValue(val);
    }
    
    clock_t t1 = clock();
    
    printf("CLOCK : %f", (float)(t1-t0) / (float)CLOCKS_PER_SEC );
    
//    print(treeRoot);
	
	return 0;
}
