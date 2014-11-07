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
#include "sdb_error.h"

FILE* out_file;

void ffatal(const char * format, ...) {
    if (format) {
        va_list arg;
        fprintf(stderr, format, arg);
        fprintf(stderr, "\n");
    }
    exit(1);
}

void init_out(const char* filename) {
    if (NULL == filename) {
        out_file = stdout;
        outf("Writing to stdout\n");
    }
    else {
        FILE * f = fopen(filename, "w");
        if (!f) {
            ffatal("Unable to open output file for writing.");
//            set_error(OUT_FILE_NOT_FOUND);
            return;
        }
        out_file = f;
    }
}

void close_out() {
    if (out_file && (out_file != stdout)) {
        if (fclose(out_file)) {
            ffatal("Could not close output file.");
//            set_error(OUT_FILE_NOT_CLOSED);
        }
    }
}

void outf(const char* format, ...) {
    if (!out_file) {
        init_out(NULL);
    }

    va_list arg;
    fprintf(out_file, format, arg);
}

void * xmalloc(size_t size) {
    const size_t alignment = 16;
    void * t;
    if (posix_memalign(&t, alignment, size) != 0) {
        ffatal("Unable to allocate enough memory.");
//        set_error(MALLOC_NOT_ENOUGH_MEM);
    }

    return t;
}

void * xrealloc(void *ptr, size_t size) {
    void * t = realloc(ptr, size);
    if (!t) {
        ffatal("Unable to allocate enough memory.");
//        set_error(REALLOC_NOT_ENOUGH_MEM);
    }
    return t;
}

#endif /* DATA_H_ */
