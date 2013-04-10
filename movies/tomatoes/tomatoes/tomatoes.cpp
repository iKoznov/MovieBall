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

#include <istream>
#include <algorithm>
#include <list>
#include <map>
#include <set>

#include <string.h>

using namespace std;

class TreeNode
{
public:
    
    struct NodeComp {
        bool operator()(const TreeNode left, const TreeNode right) const  {
            return strcmp( left.movie->id, right.movie->id ) < 0;
        }
    };
    
    struct NodeIterComp {
        bool operator()( const set<TreeNode>::const_iterator left, const set<TreeNode>::const_iterator right ) const  {
            return strcmp( (*left).movie->id, (*right).movie->id ) < 0;
        }
    };
    
    static set<TreeNode,NodeComp> NODES;
    set <set<TreeNode>::const_iterator,NodeIterComp> links;
    Movie movie;
    
//    friend bool operator==(const MovieTreeNode &left, const MovieTreeNode &right);
//    friend bool operator<(const MovieTreeNode &left, const MovieTreeNode &right);
//    friend bool operator>(const MovieTreeNode &left, const MovieTreeNode &right);
    
    static pair<set<TreeNode>::const_iterator,bool> node( Movie mov )
    {
        pair<set<TreeNode>::const_iterator,bool> p = NODES.insert( TreeNode(mov) );
        TreeNode nd = *(p.first);
//        TreeNode nd = TreeNode( mov );
        
        cout << "POINTER : " << &(*(p.first)) << endl;
        cout << "POINTER : " << &(nd) << endl;
        
        return p;
    }
    
    static TreeNode *init( Movie mov ) {
//        movie = m;
        TreeNode *node = new TreeNode( mov );
//        NODES.insert( *node );
        return node;
    }
    
    TreeNode(Movie mov)
    {
        movie = mov;
    }
    
    void similars( size_t depth )
    {
        MovieList similars = movie_similars( movie );
        struct _MovieListElem *elem = similars->first;
        while (elem) {
            TreeNode *link = init(elem->movie);
//            links.insert( pair<string, MovieTreeNode *> (link->movie->id,link) );
//            links.push_back(link);
//            elem->movie
            elem = elem->next;
        }
        MovieListFree( similars );
    }
    
    ~TreeNode() {
//        printf( "%s %s %s\n", __PRETTY_FUNCTION__, (*this).movie->id, (*this).movie->title );
//        movie_free( movie );
//        NODES.erase( *this );
    }
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

void tomatoes()
{
    puts(__PRETTY_FUNCTION__);
    
    //    int c = MovieTreeNode::count;
    
//    unsigned counter = 0;
////    while (counter < 4) {
//        Movie m = movie_make( strdup("771188157"), strdup("american+Beauty") );
//        MovieTreeNode *node = MovieTreeNode::init(m);
//        node->similars(2);
////        movie_free(m);
//        delete node;
//    
//        printf( "counter %d\n", counter++ );
////    }
//    
//    for ( set <MovieTreeNode>::iterator i = MovieTreeNode::NODES.begin(); i != MovieTreeNode::NODES.end(); i++ ) {
//        cout << "NODES " << &(*i) << " : " << (*i).movie->id << " - [ " << (*i).movie->title << " ]" << endl;
//    }
    
    TreeNode::node( movie_make( strdup("123123123"), strdup("american+Beauty") ) );
    
    struct APtrComp
    {
        bool operator()(const string lhs, const string rhs) const  {
            cout << lhs << " == " << rhs << endl;
            return (lhs) < (rhs);
        }
    };
    
    pair <set <string>::iterator, bool> p0;
    
    pair <set <string>::iterator, bool> p1;
    pair <set <string>::iterator, bool> p2;
    pair <set <string>::iterator, bool> p3;
    
    set <string> strings;
    p0 = strings.insert("String 1");
    p1 = strings.insert("String 1");
    p0 = strings.insert("String 2");
    p2 = strings.insert("String 2");
    p0 = strings.insert("String 3");
    p0 = strings.insert("String 3");
    p0 = strings.insert("String 4");
    
    
    strings.erase( p1.first );
    strings.erase( p2.first );
//    strings.erase( p3.first );
    
    for ( set <string>::iterator i = strings.begin(); i != strings.end(); i++ ) {
        cout << "NODES " << (*i) << " : " << (*i) << endl;
    }
    
    string str = *(p0.first);
    cout << "STR : " << str << endl;
    
    
    TreeNode *nodePointer = new TreeNode( movie_make( strdup("771188157"), strdup("american+Beauty") ) );
    delete nodePointer;
    
    struct NodeComp
    {
        bool operator()(const TreeNode left, const TreeNode right) const  {
//            cout << lhs << " == " << rhs << endl;
            return strcmp( left.movie->id, right.movie->id ) < 0;
        }
    };
    
    set <TreeNode,NodeComp> nodeSet;
    pair <set<TreeNode>::iterator, bool> nodePair;
    
    nodePair = nodeSet.insert( TreeNode( movie_make( strdup("771188157"), strdup("american+Beauty") ) ) );
    nodePair = nodeSet.insert( TreeNode( movie_make( strdup("771188157"), strdup("american+Beauty") ) ) );
    nodePair = nodeSet.insert( TreeNode( movie_make( strdup("1231"), strdup("american+Beauty") ) ) );
    nodePair = nodeSet.insert( TreeNode( movie_make( strdup("12"), strdup("american+Beauty") ) ) );
    nodePair = nodeSet.insert( TreeNode( movie_make( strdup("23123123"), strdup("american+Beauty") ) ) );
    nodePair = nodeSet.insert( TreeNode( movie_make( strdup("23123123"), strdup("american+Beauty") ) ) );
//    MovieTreeNode node = *nodePair.first;
//    node.movie = movie_make( strdup("771188157"), strdup("american+Beauty") );
    
    for ( set <TreeNode>::iterator i = nodeSet.begin(); i != nodeSet.end(); i++ ) {
        cout << "nodeSet " << (*i).movie->id << " " << (*i).movie->title << endl;
    }
    
//    unsigned counter = 0;
//    for ( set <MovieTreeNode>::iterator i = nodeSet.begin(); i != nodeSet.end(); i++ ) {
//        cout << "NODES " << counter << " " << &(*i) << " : " << (*i).movie->id << " - [ " << (*i).movie->title << " ]" << endl;
//    }
    
    
//    //    movie_reqId( m );
//    
//    //    printf( "        id : %s\n", m->id );
//    //    printf( "     title : %s\n", m->title );
//    
//    MovieList similars = movie_similars( m );
//    
//    struct _MovieListElem *elem = similars->first;
//    while ( elem ) {
//        Movie movie = elem->movie;
//        printf( "        id : %s\n", movie->id );
//        printf( "     title : %s\n", movie->title );
//        
//        MovieTreeNode node = MovieTreeNode(m);
//        node.NODES.insert ( pair<string, MovieTreeNode> ( movie->id, node ) );
//        
//        elem = elem->next;
//    }
//    
//    for ( map <string,MovieTreeNode>::iterator i = MovieTreeNode::NODES.begin(); i != MovieTreeNode::NODES.end(); i++ )
//    {
//        cout << "MAP : " << i->first << endl;
//    }
//    
//    MovieListFree( similars );
//    movie_free( m );
}



