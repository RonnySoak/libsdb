/*
 * util.h
 *
 *  Created on: 18 Jul 2014
 *      Author: Jakob Frielingsdorf
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef LINE_MAX
#define LINE_MAX 2048
#endif

extern int symtype;
extern int query_strands;

void * xmalloc(size_t size);
void * xrealloc(void *ptr, size_t size);

// output data
void ffatal(const char * format, ...);

/** Initialises the output stream. Default is stdout */
void init_out(const char* filename);
/** Writes to the output stream */
void outf(const char* format, ...);
// output data

// TODO what does it do???
char * xstrchrnul(char *s, int c);

// TODO what does it do???
unsigned long hash_fnv_1a_64(unsigned char * s, unsigned long n);

#endif /* UTIL_H_ */