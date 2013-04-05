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

#include <aio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "tomatoes.h"

struct _mStr {
    char *str;
    struct _mStr *next;
};

typedef struct {
    struct _mStr *first;
    struct _mStr *last;
    unsigned strLen;
} mStr;

mStr mStr_make(char *str) {
    struct _mStr *subStr = malloc(sizeof(struct _mStr));
    subStr->str = str;
    subStr->next = NULL;
    mStr s = { subStr, subStr, strlen(str) };
    return s;
}

mStr mStr_add(mStr s, char *str) {
    struct _mStr *subStr = malloc(sizeof(struct _mStr));
    subStr->str = str;
    subStr->next = NULL;
    s.last->next = subStr;
    s.last = subStr;
    s.strLen += strlen(str);
    return s;
}

char *mStr_string(mStr s) {
    char *str = calloc(s.strLen+1, sizeof(char));
    struct _mStr *p = s.first;
    while (p) {
        char *c = p->str;
        while ( ( *(str++) = *(c++) ) );
        str--;
        p = p->next;
    }
    return str - s.strLen;
}

void mStr_free();

void tomatoes()
{
    puts(__PRETTY_FUNCTION__);
    
    
    mStr myString = mStr_make(strdup("abc"));
    myString = mStr_add( myString, strdup(" Hello") );
    printf("LEN : %zd\n", myString.strLen );
    printf( "%s\n", mStr_string(myString) );
    
    struct sockaddr_in peer;
    int s;
    int rc;
    char buf[8];
    
    /* http://api.rottentomatoes.com/api/public/v1.0.json?apikey=gfh7w8cm88rvptkndqq556x4 */
    
    peer.sin_family = AF_INET;
    peer.sin_port = htons( 80 );
//    peer.sin_addr.s_addr = inet_addr( "74.125.143.138" );
    
    struct hostent *srv = gethostbyname("api.rottentomatoes.com");
//    struct hostent *srv=gethostbyname("www.nsu.ru");
    if ( !srv )
    {
        perror( "ошибка вызова gethostbyname" );
        return;
    } else puts( "вызов gethostbyname" );
    
    peer.sin_addr.s_addr = *((unsigned long*)srv->h_addr);
    
    s = socket( AF_INET, SOCK_STREAM, 0 );
    if ( s < 0 )
    {
        perror( "ошибка вызова socket" );
        return;
    } else puts( "вызов socket" );
    
    rc = connect( s, ( struct sockaddr * )&peer, sizeof( peer ) );
    if ( rc )
    {
        perror( "ошибка вызова connect" );
        return;
    } else puts( "вызов connect" );
    
    char request[] =
    "GET /api/public/v1.0.json?apikey=gfh7w8cm88rvptkndqq556x4 HTTP/1.1\r\n"
    "Host: api.rottentomatoes.com\r\n"
    "\r\n";
//    char request[] = "GET / HTTP/1.0\r\n\r\n";
    rc = send( s, request, sizeof(request), 0 );
    if ( rc <= 0 )
    {
        perror( "ошибка вызова send" );
        return;
    }
    char c[] = "\r\n";
    printf("SIZE : %d\n", sizeof(c) );
    
//    char *data ;
    while ( (rc = recv(s,buf,sizeof(buf),0) ) > 0 ) {
        for (int i = 0; i < rc; i++) putchar(buf[i]);
    }
    puts("");
    puts("окончание recv");
    return;
}



