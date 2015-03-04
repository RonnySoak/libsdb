/*
 * util.c
 *
 *  Created on: 18 Jul 2014
 *      Author: Jakob Frielingsdorf
 */

#include <string.h>
#include <stdarg.h>
#include <mm_malloc.h>

#include "util.h"
#include "sdb_error.h"

FILE* out_file;

void * sdb_malloc( size_t size ) {
    const size_t alignment = 16;
    void * t;
    if( posix_memalign( &t, alignment, size ) != 0 ) {
        sdb_add_error( MALLOC_NOT_ENOUGH_MEM );
    }

    return t;
}

void * sdb_realloc( void *ptr, size_t size ) {
    void * t = realloc( ptr, size );
    if( !t ) {
        sdb_add_error( REALLOC_NOT_ENOUGH_MEM );
    }
    return t;
}
