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

//#include <aio.h>
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <netdb.h>

#include <curl/curl.h>

#include "cJSON.h"
#include "tomatoes.h"
#include "mStr.h"

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
void curl()
{
    int wr_error = 0;
    
    /* First step, init curl */
    CURL *curl = curl_easy_init();
    if (!curl) {
        printf("couldn't init curl\n");
        return;
    }
    
    /* Tell curl the URL of the file we're going to retrieve */
    curl_easy_setopt( curl, CURLOPT_URL, "http://api.rottentomatoes.com/api/public/v1.0/movies.json?apikey=gfh7w8cm88rvptkndqq556x4&q=Toy+Story+3&_prettyprint=true" );
    curl_easy_setopt( curl, CURLOPT_URL, "http://api.rottentomatoes.com/api/public/v1.0/movies/770672122/similar.json?apikey=gfh7w8cm88rvptkndqq556x4&_prettyprint=true" );
//    770672122
    
    curl_easy_setopt( curl, CURLOPT_WRITEDATA, (void *)&wr_error );
    curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, write_data );
    
    /* Allow curl to perform the action */
    CURLcode ret = curl_easy_perform( curl );
    
    /* Emit the page if curl indicates that no errors occurred */
    if ( ret == 0 ) {
        printf( "%s\n", wr_buf );
        printf( "Recived chars : %zd\n", strlen(wr_buf) );
    }
    else return;
    
    curl_easy_cleanup( curl );
    
    cJSON *root = cJSON_Parse(wr_buf);
//    json(root,0,999);
//    puts( cJSON_Print(root) );
    cJSON_Delete(root);
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

typedef struct _Movie *Movie;

struct _Movie {
    int id;
    char *title;
};

char apikey[] = "apikey=gfh7w8cm88rvptkndqq556x4";

char *arr[] = { "apikey=gfh7w8cm88rvptkndqq556x4", "_prettyprint=true" };

Movie movie_make( int id, char *title )
{
    Movie res = malloc( sizeof(struct _Movie) );
    res->id = id;
    res->title = title;
    return res;
}

void movie_reqTitle( Movie movie )
{
    
}

Movie *movie_similars( Movie *movie )
{
    
}

void movie_free( Movie movie )
{
    free( movie->title );
}

void tomatoes()
{
    puts(__PRETTY_FUNCTION__);
    
//    curl();
    while (1) {
        mStr myString = mStr_make(strdup("abc"));
        mStr_addStr( myString, strdup(" Hello") );
        printf("LEN : %zd\n", mStr_length(myString) );
        char *str = mStr_string(myString);
        printf( "%s\n", str );
        free(str);
        mStr_free(myString);
    }
    
//    struct sockaddr_in peer;
//    int s;
//    int rc;
//    char buf[1024];
//    
//    /* http://api.rottentomatoes.com/api/public/v1.0.json?apikey=gfh7w8cm88rvptkndqq556x4 */
//    
//    peer.sin_family = AF_INET;
//    peer.sin_port = htons( 80 );
////    peer.sin_addr.s_addr = inet_addr( "74.125.143.138" );
//    
//    struct hostent *srv = gethostbyname("api.rottentomatoes.com");
////    struct hostent *srv=gethostbyname("www.nsu.ru");
//    if ( !srv )
//    {
//        perror( "ошибка вызова gethostbyname" );
//        return;
//    } else puts( "вызов gethostbyname" );
//    
//    peer.sin_addr.s_addr = *((unsigned long*)srv->h_addr);
//    
//    s = socket( AF_INET, SOCK_STREAM, 0 );
//    if ( s < 0 )
//    {
//        perror( "ошибка вызова socket" );
//        return;
//    } else puts( "вызов socket" );
//    
//    rc = connect( s, ( struct sockaddr * )&peer, sizeof( peer ) );
//    if ( rc )
//    {
//        perror( "ошибка вызова connect" );
//        return;
//    } else puts( "вызов connect" );
//    
//    char request[] =
//    "GET /api/public/v1.0.json?apikey=gfh7w8cm88rvptkndqq556x4 HTTP/1.1\r\n"
//    "Host: api.rottentomatoes.com\r\n"
//    "\r\n";
////    char request[] = "GET / HTTP/1.0\r\n\r\n";
//    rc = send( s, request, sizeof(request), 0 );
//    if ( rc <= 0 )
//    {
//        perror( "ошибка вызова send" );
//        return;
//    }
//    char c[] = "\r\n";
//    printf("SIZE : %d\n", sizeof(c) );
//    
////    char *data ;
//    while ( (rc = recv(s,buf,sizeof(buf),0) ) > 0 ) {
//        for (int i = 0; i < rc; i++) putchar(buf[i]);
//    }
//    puts("");
//    puts("окончание recv");
//    return;
}



