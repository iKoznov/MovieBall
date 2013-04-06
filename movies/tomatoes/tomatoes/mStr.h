//
//  mStr.h
//  tomatoes
//
//  Created by user on 4/6/13.
//  Copyright (c) 2013 iKoznov. All rights reserved.
//

#ifndef tomatoes_mStr_h
#define tomatoes_mStr_h


typedef struct _mStr *mStr;


mStr mStr_make( char *str );
void mStr_addArr( mStr s, char *arr, size_t len );
void mStr_addStr( mStr s, char *str );
char *mStr_string( mStr s );
size_t mStr_length( mStr s );
void mStr_free( mStr s );

#endif
