//
//  tomatoes.c
//  tomatoes
//
//  Created by user on 4/5/13.
//  Copyright (c) 2013 iKoznov. All rights reserved.
//

#include <stdio.h>

#include "tomatoes.h"
#include "mStr.h"
#include "movie.h"

#include <istream>
#include <list>
#include <set>

using namespace std;

class TreeNode
{
public:
    
    struct NodeComp {
        bool operator()(const TreeNode &left, const TreeNode &right) const  {
            return strcmp( left.movie->id, right.movie->id ) < 0;
        }
    };
    
//    struct NodeIterComp {
//        bool operator()( const set<TreeNode>::const_iterator left, const set<TreeNode>::const_iterator right ) const  {
//            return strcmp( (*left).movie->id, (*right).movie->id ) < 0;
//        }
//    };
    
    static set<TreeNode,NodeComp> NODES;
//    set <set<TreeNode>::const_iterator,NodeIterComp> links;
    Movie movie;
    
//    friend bool operator==(const MovieTreeNode &left, const MovieTreeNode &right);
//    friend bool operator<(const MovieTreeNode &left, const MovieTreeNode &right);
//    friend bool operator>(const MovieTreeNode &left, const MovieTreeNode &right);
    
    static const TreeNode &node( Movie mov )
    {
        pair<set<TreeNode>::const_iterator,bool> p = NODES.insert( TreeNode(mov) );
//        TreeNode nd = *(p.first);
////        TreeNode nd = TreeNode( mov );
//        
//        cout << "POINTER : " << &(*(p.first)) << endl;
//        cout << "POINTER : " << &(nd) << endl;
//        
//        return *(p.first);
        TreeNode *obj = new TreeNode( mov );
        return *obj;
    }
    
//    static const TreeNode &init( Movie mov ) {
////        movie = m;
//        TreeNode *node = new TreeNode( mov );
////        NODES.insert( *node );
//        return *node;
//    }
    
//    void similars( size_t depth )
//    {
//        MovieList similars = movie_similars( movie );
//        struct _MovieListElem *elem = similars->first;
//        while (elem) {
//            TreeNode link = init(elem->movie);
////            links.insert( pair<string, MovieTreeNode *> (link->movie->id,link) );
////            links.push_back(link);
////            elem->movie
//            elem = elem->next;
//        }
//        MovieListFree( similars );
//    }
    
    ~TreeNode() {
//        printf( "%s %s %s\n", __PRETTY_FUNCTION__, (*this).movie->id, (*this).movie->title );
//        movie_free( movie );
//        NODES.erase( *this );
    }
private:
    TreeNode() {
        puts( __PRETTY_FUNCTION__ );
    };
//    TreeNode(const TreeNode& root) {
//        puts( __PRETTY_FUNCTION__ );
//    };
private:
    TreeNode(Movie mov) {
        movie = mov;
    }
//    TreeNode(const TreeNode& root);
//    TreeNode& operator=(const TreeNode&);
};

//bool operator==(const MovieTreeNode &left, const MovieTreeNode &right) {
//    return strcmp(left.movie->id, right.movie->id) == 0;
//}
//
//bool operator<(const MovieTreeNode &left, const MovieTreeNode &right) {
//    return strcmp(left.movie->id, right.movie->id) < 0;
//}
//
//bool operator>(const MovieTreeNode &left, const MovieTreeNode &right) {
//    return strcmp(left.movie->id, right.movie->id) > 0;
//}

set <TreeNode,TreeNode::NodeComp> TreeNode::NODES = set <TreeNode,TreeNode::NodeComp> ();

//class A {
//    string str;
//public:
//    A(const char *s) { str = string(s); }
//    void const abc() { }
//    const string &getStr() { return str; }
//    ~A() {
//        puts(__PRETTY_FUNCTION__);
//    }
////private:
////    A(const A& root);
//    A &operator=(A const * source);
//    A(const A& root) {}
//};
//
//A & A::operator=(A const * source) {
//    return *this;
//}

class Node
{
private:
    /* banning on copy or default constructor */
    Node() { };
    Node(const Node& root);
    Node& operator=(const Node&);
    
    /* global set of all Nodes at all */
    struct NodeCmp {
        bool operator () ( Node *left,  Node *right)   {
            return strcmp( (*left)._movie->id, (*right)._movie->id ) < 0;
        }
    };
    
    static set <Node *, NodeCmp> _nodes;
    
    /* properties of each concrete Node object */
    Movie _movie;
    set <Node *> _links;
    
    /* consctuctor, that sets identity of object */
    Node(Movie &s);
    
public:
    /* static members */
    static Node& node(Movie);
    static void printNodes();
    
    /* messages that can be recieved by each object */
    void similars();
    
    /* destructor */
    ~Node();
};

set <Node *, Node::NodeCmp> Node::_nodes = set <Node *, Node::NodeCmp> ();

Node::Node(Movie &mov)
{
    _movie = mov;
    _links = set <Node *> ();
}

Node::~Node()
{
    movie_free( _movie );
}

Node& Node::node(Movie mov)
{
    Node *n = new Node(mov);
    pair <set <Node *>::iterator, bool> p = _nodes.insert(n);
    if ( !p.second ) delete n;
    return *(*p.first);
}

void Node::printNodes()
{
    cout << "------------------------------------" << endl;
    for ( set <Node *>::iterator i = _nodes.begin(); i!=_nodes.end(); i++ ) {
        cout << "[" << (*i)->_movie->id << "] - " << (*i)->_movie->title << endl;
    }
}

void Node::similars()
{
    MovieList list = movie_similars( _movie );
    MovieListFree(list);
}

void tomatoes()
{
    puts(__PRETTY_FUNCTION__);
    
    unsigned counter = 0;
    while (true)
    {
        cout << "[" << counter++ << "]";
        Node &n = Node::node( movie_make( strdup("770672122"), strdup("abc") ) );
        
        n.similars();
        
        Node::printNodes();
    }
    
//    struct Acmp {
//        bool operator()( A *left,  A *right)   {
//            return left->getStr() < right->getStr();
//        }
//    };
//    
//    set <A*,Acmp> mySet = set <A*,Acmp> ();
//    
//    A *a = new A("one");
//    cout << "STR : " << (*a).getStr() << endl;
//    
//    while (true)
//    {
//        A *a1 = new A("one"); if(!mySet.insert(a1).second) delete a1;
//        A *a2 = new A("two"); if(!mySet.insert(a2).second) delete a2;
//        A *a3 = new A("two"); if(!mySet.insert(a3).second) delete a3;
//        A *a4 = new A("tree"); if(!mySet.insert(a4).second) delete a4;
//        
//        for ( set <A*>::iterator i = mySet.begin(); i != mySet.end(); i++ ) {
//            cout << "A : " << (*i)->getStr() << endl;
//        }
//    }
//    
////    while (true) {
////        mySet.insert( new A("four") );
////    }
//    
//    for ( set <A*>::iterator i = mySet.begin(); i != mySet.end(); i++ ) {
//        cout << "A : " << (*i)->getStr() << endl;
//    }
//    
//    mySet.push_back(s2);
//    
//    TreeNode::node( movie_make( strdup("id1"), strdup("american+Beauty") ) );
//    TreeNode::node( movie_make( strdup("id2"), strdup("american+Beauty") ) );
//    TreeNode::node( movie_make( strdup("id3"), strdup("american+Beauty") ) );
//    TreeNode n = TreeNode::node( movie_make( strdup("id4"), strdup("american+Beauty") ) );
}



