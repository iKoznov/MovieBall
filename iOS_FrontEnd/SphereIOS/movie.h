//
//  movie.h
//  tomatoes
//
//  Created by user on 4/8/13.
//  Copyright (c) 2013 iKoznov. All rights reserved.
//

#ifndef tomatoes_movie_h
#define tomatoes_movie_h

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct _Movie *Movie;
struct _Movie {
    char *id;
    char *title;
    char *thumbnail;
    char *profile;
    char *detailed;
    char *original;
};

Movie movie_make( char *id, char *title );
void movie_free( Movie movie );
void movie_reqTitle( Movie movie );
void movie_reqId( Movie movie );

struct _MovieListElem {
    Movie movie;
    struct _MovieListElem *next;
};

typedef struct _MovieList *MovieList;
struct _MovieList {
    struct _MovieListElem *first;
    struct _MovieListElem *last;
    size_t count;
};

MovieList MovieListMake(Movie movie);
void MovieListAdd( MovieList list, Movie movie );
void MovieListFree( MovieList list );
MovieList movie_similars( Movie movie );

typedef struct _Poster *Poster;
struct _Poster {
    void *data;
    size_t size;
};
    
#ifdef __cplusplus
}
#endif

#endif
