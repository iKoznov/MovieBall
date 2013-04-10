//
//  File.c
//  tomatoes
//
//  Created by user on 4/8/13.
//  Copyright (c) 2013 iKoznov. All rights reserved.
//


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <curl/curl.h>

#include "cJSON.h"
#include "mStr.h"
#include "movie.h"

#define MAX_BUF	65536

char wr_buf[MAX_BUF+1];
int wr_index;

size_t write_data( void *buffer, size_t size, size_t nmemb, void *userp )
{
    puts(__PRETTY_FUNCTION__);
    
    size_t segsize = size * nmemb;
    
    /* Check to see if this data exceeds the size of our buffer. If so,
     * set the user-defined context value and return 0 to indicate a
     * problem to curl.
     */
    if ( wr_index + segsize > MAX_BUF ) {
        *(int *)userp = 1;
        return 0;
    }
    
    /* Copy the data from the curl buffer into our buffer */
    memcpy( (void *)&wr_buf[wr_index], buffer, (size_t)segsize );
    
    /* Update the write index */
    wr_index += segsize;
    
    /* Null terminate the buffer */
    wr_buf[wr_index] = 0;
    
    /* Return the number of bytes received, indicating to curl that all is okay */
    return segsize;
}

void json( cJSON *, int, int );
cJSON *curl( char *URL )
{
    puts( __PRETTY_FUNCTION__ );
    
    int wr_error = 0;
    wr_index = 0;
    
    /* First step, init curl */
    CURL *curl = curl_easy_init();
    if (!curl) {
        printf("couldn't init curl\n");
        return NULL;
    }
    
    /* Tell curl the URL of the file we're going to retrieve */
    curl_easy_setopt( curl, CURLOPT_URL, URL );
    //    curl_easy_setopt( curl, CURLOPT_URL, "http://api.rottentomatoes.com/api/public/v1.0/movies/770672122/similar.json?apikey=gfh7w8cm88rvptkndqq556x4&_prettyprint=true" );
    //    770672122
    
    curl_easy_setopt( curl, CURLOPT_WRITEDATA, (void *)&wr_error );
    curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, write_data );
    
    /* Allow curl to perform the action */
    CURLcode ret = curl_easy_perform( curl );
    
    /* Emit the page if curl indicates that no errors occurred */
    if ( ret == 0 ) {
        //        printf( "%s\n", wr_buf );
        printf( "Recived chars : %zd\n", strlen(wr_buf) );
    }
    else return NULL;
    
    curl_easy_cleanup( curl );
    
    cJSON *root = cJSON_Parse(wr_buf);
    //    json(root,0,999);
    //    puts( cJSON_Print(root) );
    //    cJSON_Delete(root);
    return root;
}

void json( cJSON *node, int inset, int maxInset )
{
    if (inset > maxInset) return;
    while (node)
    {
        char *strBuf = calloc( 100, sizeof(char) );
        char *typeStr = NULL;
        char *valueStr = NULL;
        
        for ( int i = 0; i < inset; i++) putchar('\t');
        
        switch (node->type) {
            case cJSON_False: typeStr = "cJSON_False"; break;
            case cJSON_True: typeStr = "cJSON_True"; break;
            case cJSON_NULL: typeStr = "cJSON_NULL"; break;
            case cJSON_Number: typeStr = "cJSON_Number"; sprintf(strBuf, "%d", node->valueint); valueStr = strBuf; break;
            case cJSON_String: typeStr = "cJSON_String"; valueStr = node->valuestring; break;
            case cJSON_Array: typeStr = "cJSON_Array"; break;
            case cJSON_Object: typeStr = "cJSON_Object"; break;
            default: break;
        }
        
        printf( "<%s : %s> %s\n", typeStr, node->string, valueStr );
        free( strBuf );
        
        json( node->child, inset + 1, maxInset );
        
        node = node->next;
    }
}

char apikey[] = "apikey=gfh7w8cm88rvptkndqq556x4";
char *paramsArr[] = { "apikey=gfh7w8cm88rvptkndqq556x4", "_prettyprint=true" };

Movie movie_make( char *id, char *title )
{
    Movie res = malloc( sizeof(struct _Movie) );
    res->id = id;
    res->title = title;
    return res;
}

void movie_free( Movie movie )
{
    free( movie->title );
    free( movie->id );
    free( movie );
}

void movie_reqTitle( Movie movie )
{
    
}

void movie_reqId( Movie movie )
{
    mStr _url = mStr_make( strdup("http://api.rottentomatoes.com/api/public/v1.0/movies.json?apikey=gfh7w8cm88rvptkndqq556x4&_prettyprint=true") );
    mStr_addStr( _url, strdup("&q=") );
    mStr_addStr( _url, strdup(movie->title) );
    char *url = mStr_string( _url );
    mStr_free( _url );
    puts( url );
    
    
    //    size_t cParams = sizeof( paramsArr ) / sizeof( paramsArr[0] );
    //    for ( size_t i = 0; i < cParams; i++) {
    //
    //    }
    //    printf( "params size : %ld\n", sizeof( paramsArr ) / sizeof( paramsArr[0] ) );
    char *titleValue = NULL;
    char *idValue = NULL;
    
    cJSON *tree = curl( url );
    free(url);
    if (!tree) goto errorReqId;
    
    //    json( tree, 0, 10 );
    
    cJSON *p = tree->child;
    if (!p) goto errorReqId;
    
    cJSON *total = NULL, *movies = NULL;
    while ( p ) {
        if ( strcmp( "total", p->string ) == 0 ) total = p;
        if ( strcmp( "movies", p->string ) == 0 ) movies = p;
        p = p->next;
    }
    
    if (!total || !movies) goto errorReqId;
    if ( !total->valueint ) goto errorReqId;
    if ( total->valueint < 1 ) { puts( "movie not found" ); goto cleanReqId; }
    
    cJSON *mov = movies->child;
    if (!mov) goto errorReqId;
    p = mov->child;
    if (!p) goto errorReqId;
    
    cJSON *id, *title;
    while ( p ) {
        //        puts( p->string );
        if ( strcmp( "id", p->string ) == 0 ) id = p;
        if ( strcmp( "title", p->string ) == 0 ) title = p;
        p = p->next;
    }
    
    if ( title->valuestring ) titleValue = strdup( title->valuestring );
    else goto errorReqId;
    
    if ( id->valuestring ) idValue = strdup( id->valuestring );
    else goto errorReqId;
    
    //    printf( "     total : %d\n", totalValue );
    //    printf( "        id : %d\n", idValue );
    //    printf( "     title : %s\n", title->valuestring );
    
    goto cleanReqId;
    
errorReqId:
    printf("%s : recieved JSON isn't correct", __PRETTY_FUNCTION__);
cleanReqId:
    cJSON_Delete( tree );
    if (idValue) {
        if (movie->id) free(movie->title);
        movie->id = idValue;
    }
    if (titleValue) {
        if (movie->title) free(movie->title);
        movie->title = titleValue;
    }
}

MovieList MovieListMake(Movie movie)
{
    MovieList list = malloc( sizeof(struct _MovieList) );
    list->first = NULL;
    list->last = NULL;
    list->count = 0;
    
    if ( movie )
    {
        struct _MovieListElem *listElem = malloc( sizeof(struct _MovieListElem) );
        listElem->movie = movie;
        listElem->next = NULL;
        
        list->first = listElem;
        list->last = listElem;
        list->count++;
    }
    return list;
}

void MovieListAdd( MovieList list, Movie movie )
{
    struct _MovieListElem *listElem = malloc( sizeof(struct _MovieListElem) );
    listElem->movie = movie;
    listElem->next = NULL;
    
    if ( list->last ) list->last->next = listElem;
    else list->first = listElem;
    list->last = listElem;
    
    list->count++;
}

void MovieListFree( MovieList list )
{
    struct _MovieListElem *elem = list->first;
    while (elem) {
//        movie_free(elem->movie);
//        free(elem->movie);
        struct _MovieListElem *tmp = elem;
        elem = elem->next;
        free(tmp);
    }
    free(list);
}

MovieList movie_similars( Movie movie )
{
    mStr _url = mStr_make( strdup("http://api.rottentomatoes.com/api/public/v1.0/movies/") );
    mStr_addStr( _url, strdup(movie->id) );
    mStr_addStr( _url, strdup("/similar.json") );
    mStr_addStr( _url, strdup("?apikey=gfh7w8cm88rvptkndqq556x4&_prettyprint=true") );
    char *url = mStr_string( _url );
    mStr_free( _url );
    puts( url );
    
    MovieList list = MovieListMake(NULL);
    
    cJSON *tree = curl( url );
    free(url);
    if ( !tree ) goto errorSimilars;
    
    json( tree, 0, 2 );
    
    cJSON *p = tree->child;
    if ( !p ) goto errorSimilars;
    
    cJSON *movies = NULL;
    while (p) {
        if ( strcmp( "movies", p->string ) == 0 ) movies = p;
        p = p->next;
    }
    
    if ( !movies ) goto errorSimilars;
    
    cJSON *mov = movies->child;
    while ( mov ) {
        p = mov->child;
        cJSON *id = NULL, *title = NULL;
        while (p) {
            //            puts(p->string);
            if ( strcmp( "id", p->string ) == 0 ) id = p;
            if ( strcmp( "title", p->string ) == 0 ) title = p;
            p = p->next;
        }
        
        if ( id->valuestring && title->valuestring ) {
            Movie tmpMovie = movie_make( strdup(id->valuestring), strdup(title->valuestring) );
            MovieListAdd( list, tmpMovie );
        }
        
        mov = mov->next;
    }
    
    //    printf( "--------->> %s\n", p->string );
    
    
    //    MovieListAdd( list, movie );
    //    MovieListAdd( list, movie );
    //    MovieListAdd( list, movie );
    
    goto cleanSimilars;
    
errorSimilars:
    printf("%s : recieved JSON isn't correct", __PRETTY_FUNCTION__);
cleanSimilars:
    cJSON_Delete( tree );
    return list;
}



