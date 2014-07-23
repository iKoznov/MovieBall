//
//  mStr.c
//  tomatoes
//
//  Created by user on 4/6/13.
//  Copyright (c) 2013 iKoznov. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mStr.h"

struct _mStrPart {
    char *str;
    struct _mStrPart *next;
};

struct _mStr {
    struct _mStrPart *first;
    struct _mStrPart *last;
    size_t strLen;
};


mStr mStr_make( char *str )
{
    struct _mStrPart *subStr = malloc(sizeof(struct _mStrPart));
    subStr->str = str;
    subStr->next = NULL;
    
    mStr res = malloc( sizeof(struct _mStr) );
    res->first = subStr;
    res->last = subStr;
    res->strLen = strlen(str);
    
    return res;
}

void mStr_addArr( mStr s, char *arr, size_t len )
{
    struct _mStrPart *subStr = malloc(sizeof(struct _mStrPart));
    subStr->str = arr;
    subStr->next = NULL;
    s->last->next = subStr;
    s->last = subStr;
    s->strLen += len;
}

void mStr_addStr( mStr s, char *str )
{
    return mStr_addArr( s, str, strlen(str) );
}

char *mStr_string( mStr s )
{
    char *str = calloc(s->strLen+1, sizeof(char));
    struct _mStrPart *p = s->first;
    while (p) {
        char *c = p->str;
        while ( ( *(str++) = *(c++) ) );
        str--;
        p = p->next;
    }
    return str - s->strLen;
}

size_t mStr_length( mStr s )
{
    return s->strLen;
}

void mStr_free( mStr s )
{
    struct _mStrPart *str = s->first;
    while (str) {
        free(str->str);
        struct _mStrPart *tmp = str;
        str = str->next;
        free(tmp);
    }
    free(s);
}

