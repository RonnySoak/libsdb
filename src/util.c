/*
 * util.c
 *
 *  Created on: 18 Jul 2014
 *      Author: Jakob Frielingsdorf
 */

#ifndef DATA_H_
#define DATA_H_

#include <string.h>
#include <stdarg.h>
#include <mm_malloc.h>

#include "util.h"

FILE* out_file;
/* Describes, if the DB sequences should be translated prior to the alignments.
 * One of: 0 - 4
 * @see sdb_init_symbol_translation in libsdb.h */
int symtype;
/* Describes which strands are used in the alignments
 * One of: 1 - 3
 * @see sdb_init_symbol_translation in libsdb.h */
int query_strands;
int db_gencode;

void ffatal(const char * format, ...) {
    if (format) {
        va_list arg;
        fprintf(stderr, format, arg);
        fprintf(stderr, "\n");
    }
    exit(1);
}

void outf(const char* format, ...) {
    va_list arg;
    fprintf(out_file, format, arg);
}

void init_out(const char* filename) {
    if(NULL == filename) {
        out_file = stdout;
        outf("Writing to stdout\n");
    }
    else {
        FILE * f = fopen(filename, "w");
        if (!f) {
            ffatal("Unable to open output file for writing.");
        }
        out_file = f;
    }
}

void * xmalloc(size_t size) {
    const size_t alignment = 16;
    void * t;
    if (posix_memalign(&t, alignment, size) != 0) {
        ffatal("Unable to allocate enough memory.");
    }

    return t;
}

void * xrealloc(void *ptr, size_t size) {
    void * t = realloc(ptr, size);
    if (!t) {
        ffatal("Unable to allocate enough memory.");
    }
    return t;
}

/* Find the first occurrence of C in S or the final NUL byte.
 * TODO why do we use an own function instead of the buildin: strchrnul in string.h ???
 *
 * strchrnul is a GNU extension!
 * */
char * xstrchrnul(char *s, int c) {
    char * r = strchr(s, c); // returns the first occurrence of c in s

    if (r)
        return r;
    else
        return (char *) s + strlen(s); // returns the final NUL byte
}

///* Find the first occurrence of C in S or the final NUL byte.  */
//char *
//strchrnul (const char *s, int c_in)
//{
//  char c = c_in;
//  while (*s && (*s != c))
//    s++;
//
//  return (char *) s;
//}

/**
 * Simple hash function used for hash tables.
 *
 * TODO remove
 */
unsigned long hash_fnv_1a_64(unsigned char * s, unsigned long n) {
    const unsigned long fnv_offset = 14695981039346656037UL;
    const unsigned long fnv_prime = 1099511628211;

    unsigned long hash = fnv_offset;

    for (unsigned long i = 0; i < n; i++) {
        unsigned char c = *s++;
        hash = (hash ^ c) * fnv_prime;
    }

    return hash;
}

#endif /* DATA_H_ */
