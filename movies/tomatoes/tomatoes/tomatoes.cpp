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

using namespace std;

class MovieTreeNode
{
public:
    static map < string, MovieTreeNode > NODES;
    list<MovieTreeNode *>links;
    Movie movie;
    
    static MovieTreeNode *init(Movie m) {
//        movie = m;
        MovieTreeNode *node = new MovieTreeNode;
        node->movie = m;
        return node;
    }
    
    void similars(size_t depth)
    {
        MovieList similars = movie_similars( movie );
        struct _MovieListElem *elem = similars->first;
        while (elem) {
            MovieTreeNode *link = init(elem->movie);
            links.push_back(link);
//            elem->movie
            elem = elem->next;
        }
        MovieListFree( similars );
    }
    
    ~MovieTreeNode() {
        movie_free( movie );
    }
private:
    MovieTreeNode(){};
    MovieTreeNode(const MovieTreeNode& root);
    MovieTreeNode& operator=(const MovieTreeNode&);
};

map <string,MovieTreeNode> MovieTreeNode::NODES = map <string,MovieTreeNode> ();

void tomatoes()
{
    puts(__PRETTY_FUNCTION__);
    
    //    int c = MovieTreeNode::count;
    
    while (true) {
        Movie m = movie_make( strdup("771188157"), strdup("american+Beauty") );
        MovieTreeNode *node = MovieTreeNode::init(m);
        node->similars(2);
//        movie_free(m);
        delete node;
    }
    
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



