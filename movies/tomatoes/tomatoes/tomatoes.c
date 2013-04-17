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

class Node
{
private:
    /* set ban on copying or default constructor */
    Node();
    Node(const Node& root);
    Node& operator=(const Node&);
    
    /* global set of all Nodes at all */
    struct NodeCmp {
        bool operator()( const Node& lhs, const Node& rhs ) const {
//            return strcmp( (left)._movie->id, (right)._movie->id ) < 0;
            return true;
        }
    };
    
    static set <Node, NodeCmp> _nodes;
    
    /* properties of each concrete Node object */
    string ID;
//    set <Node *> _links;
    
    /* consctuctor, that sets identity of object */
    Node(string &id);
    
public:
    /* static members */
    static const Node& node(string&);
//    static void printNodes();
    
    /* messages that can be recieved by each object */
//    void similars();
    
    /* destructor */
    ~Node();
};

set <Node, Node::NodeCmp> Node::_nodes = set <Node, Node::NodeCmp> ();

Node::Node(string &id)
{
    ID = id;
}

Node::~Node() { }

const Node& Node::node(string& id)
{
    Node* n=new Node(id);
    pair <set <Node>::iterator, bool> p = _nodes.insert( *n );
//    if ( !p.second ) delete n;
    return (*p.first);
}

//void Node::printNodes()
//{
//    cout << "------------------------------------" << endl;
//    for ( set <Node>::iterator i = _nodes.begin(); i!=_nodes.end(); i++ ) {
//        cout << "[" << (*i)._movie->id << "] - " << (*i)._movie->title << endl;
//    }
//}
//
//void Node::similars()
//{
//    MovieList list = movie_similars( _movie );
//    MovieListFree(list);
//}

void tomatoes()
{
    puts(__PRETTY_FUNCTION__);
    
    unsigned counter = 0;
    while (true)
    {
        cout << "[" << counter++ << "]";
        string id = "123";
        Node::node( id );
        
//        n.similars();
        
//        Node::printNodes();
    }
}



