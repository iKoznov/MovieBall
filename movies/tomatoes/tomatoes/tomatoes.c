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

//#include <istream>
//#include <list>
//#include <set>

//using namespace std;
//
//class Node
//{
//private:
//    /* set ban on copying or default constructor */
//    Node();
//    Node(const Node& root);
//    Node& operator=(const Node&);
//    
//    /* global set of all Nodes at all */
//    struct NodeCmp {
//        bool operator()( const Node& lhs, const Node& rhs ) const {
////            return strcmp( (left)._movie->id, (right)._movie->id ) < 0;
//            return true;
//        }
//    };
//    
//    static set <Node, NodeCmp> _nodes;
//    
//    /* properties of each concrete Node object */
//    string ID;
////    set <Node *> _links;
//    
//    /* consctuctor, that sets identity of object */
//    Node(string &id);
//    
//public:
//    /* static members */
//    static const Node& node(string&);
////    static void printNodes();
//    
//    /* messages that can be recieved by each object */
////    void similars();
//    
//    /* destructor */
//    ~Node();
//};
//
//set <Node, Node::NodeCmp> Node::_nodes = set <Node, Node::NodeCmp> ();
//
//Node::Node(string &id)
//{
//    ID = id;
//}
//
//Node::~Node() { }
//
//const Node& Node::node(string& id)
//{
//    Node* n=new Node(id);
//    pair <set <Node>::iterator, bool> p = _nodes.insert( *n );
////    if ( !p.second ) delete n;
//    return (*p.first);
//}
//
////void Node::printNodes()
////{
////    cout << "------------------------------------" << endl;
////    for ( set <Node>::iterator i = _nodes.begin(); i!=_nodes.end(); i++ ) {
////        cout << "[" << (*i)._movie->id << "] - " << (*i)._movie->title << endl;
////    }
////}
////
////void Node::similars()
////{
////    MovieList list = movie_similars( _movie );
////    MovieListFree(list);
////}

struct _Set {
    
};

static Set NODES;



void MovieNodeDelete(Node *node) {
    MovieNode movNd = nodeData(node);
    movie_free(movNd->movie);
    free(movNd);
    deleteNode(&NODES, node);
}

MovieNode NodeByMovie(Movie movie)
{
    MovieNode node = malloc( sizeof(struct _MovieNode) );
    node->movie = movie;
    Node *res = findNode( &NODES, node );
    if (res) {
        free(node);
        movie_free(movie);
//        MovieNodeDelete(res);
        return nodeData(res);
//        return NULL;
    };
    insertNode( &NODES, node );
    node->links = SetMake();
    return node;
}

char *MovieNodeId(MovieNode node) {
    return node->movie->id;
}

void tomatoes()
{
    puts(__PRETTY_FUNCTION__);
    
//    insertNode( MovieNO );
    printf("RAND_MAX : %d\n", RAND_MAX);
    srand( (unsigned)time(NULL) );
    
    NODES = SetMake();
    
    Movie mov = movie_make( strdup("770672122"), strdup("abc") );
    MovieNode node = NodeByMovie(mov);
    
    MovieList list = movie_similars(mov);
    struct _MovieListElem *elem = list->first;
    
    while (elem) {
        Movie same = elem->movie;
        MovieNode linked = NodeByMovie(same);
        
        printf("[%s] - %s\n", same->id, same->title);
        
        elem = elem->next;
    }
    
    print(NODES);
    
    Node *iter = firstNode( &NODES );
    while(iter) {
        MovieNode nd = nodeData( iter );
        printf("%s\n", nd->movie->id);
        iter = nextNode(iter);
//    nd->movie->id
    }
    
//    
//    unsigned counter = 0;
//    while (1)
//    {
//        char *id = calloc( 41, sizeof(char) );
//        sprintf( id, "%d", rand()%1000000 );
//        Movie mov = movie_make( id, strdup("abc") );
//        MovieNode node = NodeByMovie(mov);
//        
//        MovieNode node = malloc( sizeof(struct _MovieNode) );
//        node->movie = mov;
//        
//        insertNode(NODES,node);
//        
//        Node *t;
//        if ((t = findNode(NODES,node)) != NULL) {
////            MovieNode nd = nodeData(t);
////            movie_free(nd->movie);
////            deleteNode(NODES,t);
//            free(node);
//            movie_free(mov);
//        } else {
//            insertNode(NODES,node);
//        }
//        
//        free(node);
//        movie_free(mov);
//        
//        printf("%d\n", counter++);
//    }
}



