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

void * sdb_malloc(size_t size);
void * sdb_realloc(void *ptr, size_t size);

#endif /* UTIL_H_ */
